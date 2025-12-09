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
        return 1;
    }


    char buffer[MAX_PATH];   // tampon 
    DWORD size = MAX_PATH;

    if (QueryFullProcessImageNameA(h, 0, buffer, &size)) {
        std::cout << "Chemin du processus : " << buffer << std::endl;
    }
    else {
        std::cout << "Impossible d'obtenir le chemin. Erreur : " << GetLastError() << std::endl;
    };
    CloseHandle(h);  // TOUJOURS FERMER

    return 0;

    /*
    si val de la forme 0x..... alors Windows nous a donner un handle qui nous sera utile dans la suite pour receuillir 
    des infos tels que (le nom du processus, l'utilisation en mémoire, ...)
    */
}