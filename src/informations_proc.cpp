#include <iostream>
#include <string>
#include <Windows.h>
#include <psapi.h>
#include <TlHelp32.h>
#include "../include/liste_processus.h"





//ETAT
void affichage_etat_proc(HANDLE proc, DWORD & code_sortie) {
    if (GetExitCodeProcess(proc, &code_sortie)) {
        if (code_sortie == STILL_ACTIVE) {   // STILL_ACTIVE est defenie comme :     define STILL_ACTIVE 259
            std::cout << "Processus toujours en cours d'execution." << std::endl;
        }
        else { 
            std::cout << "Processus terminee. Code de sortie : " << code_sortie << std::endl;
        };
    }
    else affichage_erreur(GetLastError());
}

//PATH
void affichage_chemin_proc(HANDLE proc, char * tampon, DWORD & taille) {
    
    if (QueryFullProcessImageNameA(proc, 0, tampon, &taille)) {
        std::cout << "Chemin : " << tampon << std::endl;
    }
    else affichage_erreur(GetLastError());
}

//NOM EXE
//std::string extraction_nom_exe(char * tampon, DWORD taille);  grace a liste_processus.h


//Nombre de threads
DWORD get_nombre_threads(DWORD pid) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;

    THREADENTRY32 te;
    te.dwSize = sizeof(THREADENTRY32);

    DWORD compteur = 0;

    if (Thread32First(snapshot, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                ++compteur;
            }
        } while (Thread32Next(snapshot, &te));
    }

    CloseHandle(snapshot);
    return compteur;
}



int main() {
    char choix = 'o';
    do {
        std::cout << "Section precedente 'Liste des processus du systeme' pour trouver le PID du processus desire" << std::endl;
        HANDLE proc = openProcess_pid();
        if (!proc) {
            std::cout << "Echec de l'obtention du handle." << std::endl;
            return 1;
        };
        char tampon_proc[MAX_PATH];
        DWORD taille_tampon_proc = MAX_PATH;
        DWORD pid = GetProcessId(proc);


        std::cout << "==============================================================================================================" << std::endl;
        std::cout << "========================================= Information du processus " << pid << " ======================================" << std::endl;
        std::cout << "==============================================================================================================" << std::endl;
        std::cout << std::endl;

        affichage_chemin_proc(proc, tampon_proc, taille_tampon_proc);
        std::string nom_exe = extraction_nom_exe(tampon_proc, taille_tampon_proc);
        std::string droits_acces = "PROCESS_QUERY_LIMITED_INFORMATION";             //A MODIFIER SI CHANGEMENT MOYEN OUVERTURE voir 'openProcess_pid() ligne65 dans liste_processus.cpp'

        std::cout << "Nom executable : " << nom_exe << std::endl;
        std::cout << "Droits d'acces : " << droits_acces << std::endl;
        DWORD nb_threads = get_nombre_threads(pid);
        std::cout << "Nombre de threads : " << nb_threads << std::endl;

        CloseHandle(proc);


        std::cout << "Saisir un nouveau PID (o=oui n=non): ";
        std::cin >> choix;
    } while ((choix == 'o') || (choix == 'O'));
    return 0;
}