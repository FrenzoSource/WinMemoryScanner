#include <iostream>
#include <Windows.h>



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

//---------------------Mise à Jour ------------------------

void affichage_chemin_proc(HANDLE proc, char * tab_chemin, DWORD taille_tab) {
    if (QueryFullProcessImageNameA(proc, 0, tab_chemin, &taille_tab)) {
        std::cout << "chemin : " << tab_chemin << std::endl;
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
        affichage_erreur(GetLastError());       //GetLastError retourne un code d'erreur de type DWORD
        return 1;
    }

//-----------------------Mise à Jour----------------------------------
    //MAX_PATH est défini dans la WinApi et vaut 260
    char tab_chemin[MAX_PATH];
    DWORD taille_tab_chemin = MAX_PATH;
    affichage_chemin_proc(proc1, tab_chemin, taille_tab_chemin);

    DWORD code_sortie = 0;
    affichage_etat_proc(proc1, code_sortie);



    CloseHandle(proc1);     //toujours important

    return 0;
}