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
        affichage_erreur(GetLastError());
        return 1;
    }


    std::cout << "Obtention d'un HANDLE pour le processus réussit." << std::endl;




    return 0;
}