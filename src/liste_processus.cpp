#include <iostream>
#include <fstream>
#include <Windows.h>
#include <psapi.h>      //nous sert a observer et analyser les processus Windows
#include <stack>



//ouverture d'un processus grace au PID
HANDLE openProcess_pid() {
    DWORD pid;
    std::cout << "Veuillez saisir un pid : ";
    std::cin >> pid;

    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);  //ajout de LIMITED pour acceder a plus de processus

    return h;
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



void affichage_chemin_proc(HANDLE proc, char * tampon, DWORD taille) {
    
    if (QueryFullProcessImageNameA(proc, 0, tampon, &taille)) {
        std::cout << "Chemin : " << tampon << std::endl;
        std::cout << "Nom executable : " << extraction_nom_exe(tampon, taille) << std::endl;
    }
    else affichage_erreur(GetLastError());
}



void affichage_etat_proc(HANDLE proc, DWORD & code_sortie) {
    if (GetExitCodeProcess(proc, &code_sortie)) {
        if (code_sortie == STILL_ACTIVE) {   // STILL_ACTIVE est defenie comme :     define STILL_ACTIVE 259
            std::cout << "Processus toujours en cours d'execution." << std::endl;
        }
        else std::cout << "Processus terminee. Code de sortie : " << code_sortie << std::endl;
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




int main() {
//----------------------------------------------ENUMERER TOUT LES PROCESSUS DE LA MACHINE--------------------------------------------------------
    DWORD tabPids[1024];    //tableau de PID
    DWORD nbre_proc = 0;

    if (listage_processus(tabPids, 1024, &nbre_proc)) {
        std::cout << "Nombre de processus en cours d'execution : " << nbre_proc << std::endl;
        for (DWORD i = 0; i < nbre_proc; ++i) {
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
                affichage_chemin_proc(h_tmp, tampon_proc1, taille_tampon_proc1);

                DWORD code_sortie = 0;
                affichage_etat_proc(h_tmp, code_sortie);
            };

            CloseHandle(h_tmp);
        };
    };
    

    return 0;
}