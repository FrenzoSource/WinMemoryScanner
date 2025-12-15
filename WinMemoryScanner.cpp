#include <iostream>
#include <Windows.h>




//ouverture d'un processus grace au PID
HANDLE openProcess_pid() {
    DWORD pid;
    std::cout << "Veuillez saisir un pid : ";
    std::cin >> pid;

    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, false, pid);

    return h;
}




void affichage_erreur(DWORD num_erreur) {
    std::cout << "ERREUR !!" << std::endl;
    if (num_erreur == 5) std::cout << "Acces refusée. Vous ne disposez pas les droits necessaires." << std::endl;
    else if (num_erreur == 87) std::cout << "PID ou parametre invalides" << std::endl;
    else if (num_erreur == 299) std::cout << "Acces memoire limite par protection." << std::endl;
}


void affichage_chemin_proc(HANDLE proc, char * tampon, DWORD taille) {
    
    if (QueryFullProcessImageNameA(proc, 0, tampon, &taille)) {
        std::cout << "Chemin : " << tampon << std::endl;
    }
    else affichage_erreur(GetLastError());
}



void affichage_etat_proc(HANDLE proc, DWORD & code_sortie) {
    if (GetExitCodeProcess(proc, &code_sortie)) {
        if (code_sortie == STILL_ACTIVE) {   // STILL_ACTIVE est défénie comme :     define STILL_ACTIVE 259
            std::cout << "Processus toujours en cours d'execution." << std::endl;
        }
        else std::cout << "Processus terminée. Code de sortie : " << code_sortie << std::endl;
    }
    else affichage_erreur(GetLastError());
}



int main() {

    //ouverture d'un processus  pour récuperer un HANDLE
    HANDLE proc1 = openProcess_pid();
    if (proc1 == NULL) {    //Important pour la gestion des erreurs
        affichage_erreur(GetLastError());
        return 1;
    }



    //MAX_PATH est défini dans la WinApi et vaut 260
    char tampon_proc1[MAX_PATH];
    DWORD taille_tampon_proc1 = MAX_PATH;

    affichage_chemin_proc(proc1, tampon_proc1, taille_tampon_proc1);


    
    DWORD exitCode = 0;
    affichage_etat_proc(proc1, exitCode);



    CloseHandle(proc1);  // TOUJOURS FERMER

    return 0;

    /*
    si val de la forme 0x..... alors Windows nous a donner un handle qui nous sera utile dans la suite pour receuillir 
    des infos tels que (le nom du processus, l'utilisation en mémoire, ...)
    */
}