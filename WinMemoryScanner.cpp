#include <iostream>
#include <Windows.h>





int main() {
    DWORD pid;
    std::cout << "Veuillez saisir le PID du processus : ";
    std::cin >> pid;

    //ouverture du processus
    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, false, pid);

    if (h == NULL) {
        std::cout << "Echec. Erreur : " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Succèes le handle est : " << h << std::endl;   
        CloseHandle(h);  // TOUJOURS FERMER
    };
    return 0;

    /*
    si val de la forme 0x..... alors Windows nous a donner un handle qui nous sera utile dans la suite pour receuillir 
    des infos tels que (le nom du processus, l'utilisation en mémoire, ...)
    */
}