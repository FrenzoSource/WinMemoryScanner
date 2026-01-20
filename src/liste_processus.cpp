#include <iostream>
#include <fstream>
#include <Windows.h>
#include <psapi.h>
#include <stack>


//Pour le stocakge des info -> sera utiliser par la suite pour l'export de data
const int TMAX = 10000;
struct process_info{
    DWORD pid;
    std::string chemin;
    std::string nom_exe;
    std::string architecture;
    std::string etat;
};

struct maillon {
    process_info proc;
    maillon * suivant;
};
using Tab_info_proc = maillon *;


void ajout_proc_info(Tab_info_proc &tab, process_info proc_info) {
    maillon* nouveau = new maillon;
    nouveau->proc = proc_info;
    nouveau->suivant = nullptr;

    if (tab == nullptr) {
        tab = nouveau;
    } else {
        maillon* tmp = tab;
        while (tmp->suivant != nullptr)
            tmp = tmp->suivant;
        tmp->suivant = nouveau;
    }
}

void libere_tab(Tab_info_proc &tab) {
    while (tab != nullptr) {
        maillon* tmp = tab;
        tab = tab->suivant;
        delete tmp;
    }
}

//TEST
void affichage_tab_info(Tab_info_proc tab) {
    if (tab != nullptr) {
        std::cout << "Pid : " << tab->proc.pid << std::endl;
        std::cout << "Chemin : " << tab->proc.chemin << std::endl;
        std::cout << "nom exe : " << tab->proc.nom_exe << std::endl;
        std::cout << "architecture : " << tab->proc.architecture << std::endl;
        std::cout << "etat : " << tab->proc.etat << std::endl;
        affichage_tab_info(tab->suivant);
    };
}

HANDLE openProcess_pid() {
    DWORD pid;
    std::cout << "Veuillez saisir un pid : ";
    std::cin >> pid;

    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pid);

    return h;
}

//Utile si IsWow64Process2 n'est pas disponible sur la machine
typedef BOOL (WINAPI* IsWow64Process2_t) (  //Signature officielle de IsWowProcess2
    HANDLE,
    USHORT*,
    USHORT*
);

/*Fonction de chargement dynamique
    Cherche la fonction IsWow64Process2 dans windows
    -si elle existe -> renvoie un pointeur valide vers une fonction utilisable
    -si elle n'existe pas -> renvoie un NULL
*/
IsWow64Process2_t get_IsWow64Process2() {
    static IsWow64Process2_t fn = 
        (IsWow64Process2_t)GetProcAddress(    //Cherche l'adresse de la fonction par son nom puis cast vers IsWow64Process2_t
            GetModuleHandleA("kernel32.dll"),
            "IsWow64Process2"
        );
    return fn;
}



void affichage_erreur(DWORD num_erreur) {
    std::cout << "ERREUR !!" << std::endl;
    if (num_erreur == 5) std::cout << "Acces refusee. Vous ne disposez pas les droits necessaires." << std::endl;
    else if (num_erreur == 87) std::cout << "PID ou parametre invalides" << std::endl;
    else if (num_erreur == 299) std::cout << "Acces memoire limite par protection." << std::endl;
}



std::string extraction_nom_exe(char * tampon, DWORD taille) {
    std::string chaine = "";
    std::stack<char> lettre_chemin;
    int n = taille - 1;
    while ((n >= 0) and (tampon[n] != '\\')) {
        lettre_chemin.push(tampon[n]);
        --n;
    };

    while (!lettre_chemin.empty()) {
        chaine += lettre_chemin.top();
        lettre_chemin.pop();
    };
    return chaine;
}



void affichage_chemin_proc(HANDLE proc, char * tampon, DWORD taille, process_info & proc_info) {
    
    if (QueryFullProcessImageNameA(proc, 0, tampon, &taille)) {
        std::cout << "Chemin : " << tampon << std::endl;
        proc_info.nom_exe = extraction_nom_exe(tampon, taille);
        std::cout << "Nom executable : " << proc_info.nom_exe << std::endl;
    }
    else affichage_erreur(GetLastError());
}



void affichage_etat_proc(HANDLE proc, DWORD & code_sortie, process_info & proc_info) {
    if (GetExitCodeProcess(proc, &code_sortie)) {
        if (code_sortie == STILL_ACTIVE) {   // STILL_ACTIVE est defenie comme :     define STILL_ACTIVE 259
            std::cout << "Processus toujours en cours d'execution." << std::endl;
            proc_info.etat = "Processus toujours en cours d'execution.";
        }
        else { 
            std::cout << "Processus terminee. Code de sortie : " << code_sortie << std::endl;
            proc_info.etat = "Processus terminé";
        };
    }
    else affichage_erreur(GetLastError());
}




DWORD listage_processus(DWORD * tab_pid, DWORD taille_max, DWORD * nb_processus) {
    DWORD octets_retourne = 0;

    if (!EnumProcesses(tab_pid, taille_max * sizeof(DWORD), &octets_retourne)) {
        std::cout << "Echec de EnumProcesses." << std::endl;
        return 0; // echec
    }

    *nb_processus = octets_retourne / sizeof(DWORD);
    return 1; // succès
}



//pour toute info concernant WOW64 ---->     https://learn.microsoft.com/en-us/windows/win32/winprog64/running-32-bit-applications
void architecture_processus(HANDLE proc, process_info & proc_info) {
    // Tentative avec API moderne
    IsWow64Process2_t fnIsWow64Process2 = get_IsWow64Process2();
    if (fnIsWow64Process2) {
        USHORT procMachine = 0;         //Architecture du processus
        USHORT nativeMachine = 0;       //Architecture du système

        if (!fnIsWow64Process2(proc, &procMachine, &nativeMachine)) {
            std::cout << "Erreur IsWow64Process2. Code: " << GetLastError() << std::endl;
            return;
        }

        if (procMachine == IMAGE_FILE_MACHINE_I386) {
            std::cout << "Processus cible : x86 (WOW64)" << std::endl;
            proc_info.architecture = "x86";
        }
        else if (procMachine == IMAGE_FILE_MACHINE_UNKNOWN && nativeMachine == IMAGE_FILE_MACHINE_AMD64) {
            std::cout << "Processus cible : x64" << std::endl;
            proc_info.architecture = "x64";
        }
        else if (procMachine == IMAGE_FILE_MACHINE_UNKNOWN && nativeMachine == IMAGE_FILE_MACHINE_ARM64) {
            std::cout << "Processus cible : ARM64" << std::endl;
            proc_info.architecture = "ARM64";
        }
        else {
            std::cout << "Processus cible : architecture inconnue" << std::endl;
            proc_info.architecture = "Architecture inconnue.";
        };
        return;
    }

    // Cette partie ne s'execute que si IsWow64Process2 n'existe pas
    BOOL isWow64 = FALSE;
    if (!IsWow64Process(proc, &isWow64)) {
        std::cout << "Erreur IsWow64Process. Code : "
                  << GetLastError() << std::endl;
        return;
    }

    BOOL isCurrentWow64 = FALSE;
    IsWow64Process(GetCurrentProcess(), &isCurrentWow64);

    if (isCurrentWow64) {
        // OS 64 bits, notre processus est 32 bits
        if (isWow64) {
            std::cout << "Processus cible : x86 (32 bits - WOW64)" << std::endl;
            proc_info.architecture = "x86(WOW64)";
        } else {
            std::cout << "Processus cible : x64 (64 bits)" << std::endl;
            proc_info.architecture = "x64";
        }
    } 
    else {
        // OS 32 bits ou processus natif
        std::cout << "Processus cible : x86 (OS 32 bits ou natif)" << std::endl;
        proc_info.architecture = "x86 (OS 32 bits ou natif)";
    }

}



void creation_fic(Tab_info_proc tab_info, std::string nom_fic, DWORD nbre_proc) {
    std::ofstream outfile(nom_fic);
    outfile << "==============================================================================================================" << std::endl;
    outfile << "=========================================== LISTE DES PROCESSUS ==============================================" << std::endl;
    outfile << "==============================================================================================================" << std::endl;
    outfile << std::endl;

    outfile << "REMARQUE : Les champs vides signifient que l'accès au processus a été refusé par Windows pour des raisons de sécurité ou de protection du système." << std::endl;

    outfile << "Nombre de processus : " << nbre_proc << std::endl;
    while (tab_info != nullptr) {
        outfile << "_____________" << std::endl;
        outfile << "Pid : " << tab_info->proc.pid << std::endl;
        outfile << "Chemin : " << tab_info->proc.chemin << std::endl;
        outfile << "Nom de l'executable : " << tab_info->proc.nom_exe << std::endl;
        outfile << "Etat actuel du processus : " << tab_info->proc.etat << std::endl;
        outfile << "Architecture du processus : " << tab_info->proc.architecture << std::endl;
        outfile << std::endl;
        tab_info = tab_info->suivant;
    };
    
    outfile << "Pour toute information concernant WOW64 veuillez consulter le site : https://learn.microsoft.com/en-us/windows/win32/winprog64/running-32-bit-applications" << std::endl;

}



void choix_export(Tab_info_proc tab_info, DWORD nbre_proc) {
    char choix;
    std::cout << "Exporter les donnees dans un fichier texte ?" << std::endl;
    do {
        std::cout << "Veuillez saisir 'o' ou 'n' (o=oui   n=non) : ";
        std::cin >> choix;
    } while ((choix != 'o') and (choix != 'n') and (choix != 'O') and (choix != 'N'));
    if ((choix == 'o') or (choix == 'O')) {
        std::string fic_data = "../output_data/data_liste_proc.txt";
        creation_fic(tab_info, fic_data, nbre_proc);
        std::cout << "Le fichier de donnees se situe dans le dossier \'../output_data\'" << std::endl;
    };
}



int main() {
//----------------------------------------------ENUMERER TOUT LES PROCESSUS DE LA MACHINE--------------------------------------------------------
    DWORD tabPids[1024];    //tableau de PID
    DWORD nbre_proc = 0;
    Tab_info_proc tab_info = nullptr;

    if (listage_processus(tabPids, 1024, &nbre_proc)) {
        std::cout << "Nombre de processus en cours d'execution : " << nbre_proc << std::endl;
    
        for (DWORD i = 0; i < nbre_proc; ++i) {
            process_info proc_info;
            proc_info.pid = tabPids[i];
            std::cout << "___" << std::endl;
            std::cout << "PID: " << tabPids[i] << "   ";
            HANDLE h_tmp = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, tabPids[i]);
            if (h_tmp == NULL) {
                affichage_erreur(GetLastError());
            }
            else {
                //recreation du tableau de chemin a chaque iteration
                char tampon_proc1[MAX_PATH];
                DWORD taille_tampon_proc1 = MAX_PATH;
                affichage_chemin_proc(h_tmp, tampon_proc1, taille_tampon_proc1, proc_info);
                proc_info.chemin = tampon_proc1;
                architecture_processus(h_tmp, proc_info);  

                DWORD code_sortie = 0;
                affichage_etat_proc(h_tmp, code_sortie, proc_info);
            };
            ajout_proc_info(tab_info, proc_info);


            CloseHandle(h_tmp);
        };
    };
    choix_export(tab_info, nbre_proc);

    /*
    std::cout << std::endl;
    std::cout << "TAB INFO :" << std::endl;
    affichage_tab_info(tab_info);
    */
    libere_tab(tab_info);

    return 0;
}