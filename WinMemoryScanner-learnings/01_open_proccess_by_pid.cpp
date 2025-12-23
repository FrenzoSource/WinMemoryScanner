/*
Ce programme permet d'obtenir un HANDLE d'un processus demandé.
Un HANDLE est une référence qu'un système d'exploitation donné à 
un programme pour lui permettre d'interagir avec un processus.

On peut le voir comme un ticket d'acces donner par le système :
  - le handle représente le ticket
  - le processus représente le spectacle
Ainsi on peut acceder au spectacle grace au ticket.

Pour obtenir ce fameux handle le programme demande à l'utilisateur un PID (Process identifer), 
Ce dernier peut être trouver grace au gestionnaire des taches.
*/



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






int main() {
    //ouverture d'un processus  pour récuperer un HANDLE
    HANDLE proc1 = openProcess_pid();
    if (proc1 == NULL) {    //Important pour la gestion des erreurs
        affichage_erreur(GetLastError());       //GetLastError retourne un code d'erreur de type DWORD
        return 1;
    }


    std::cout << "Obtention d'un HANDLE pour le processus réussit." << std::endl;


    CloseHandle(proc1);     //toujours important

    return 0;
}