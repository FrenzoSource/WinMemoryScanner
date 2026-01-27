#include <iostream>
#include "../include/informations_system.h"
#include "../include/liste_processus.h"








void afffichage_actions () {
    std::cout << "0)_Help" << std::endl;
    std::cout << "1)_Informations du systeme" << std::endl;
    std::cout << "2)_Liste des processus du systeme" << std::endl;
}

void afficher_logo() {
    std::cout << R"(
     __        ___       __  __                                 ____                                  
     \ \      / (_)_ __ |  \/  | ___ _ __ ___   ___  _ __ _   _/ ___|  ___ __ _ _ __  _ __   ___ _ __ 
      \ \ /\ / /| | '_ \| |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | \___ \ / __/ _` | '_ \| '_ \ / _ \ '__|
       \ V  V / | | | | | |  | |  __/ | | | | | (_) | |  | |_| |___) | (_| (_| | | | | | | |  __/ |   
        \_/\_/  |_|_| |_|_|  |_|\___|_| |_| |_|\___/|_|   \__, |____/ \___\__,_|_| |_|_| |_|\___|_|   
                                                          |___/                                     
    
                                                          )";
}



void affichage_credit() {
    std::cout << "WinMemoryScanner v1.0" << std::endl;
    std::cout << "Auteur : FrenzoSource" << std::endl;
    std::cout << "Langage : C++" << std::endl;
    std::cout << "Depot GitHub : " << std::endl;
    std::cout << "https://github.com/FrenzoSource/WinMemoryScanner" << std::endl;
}



int main() {
    afficher_logo();
    affichage_credit();
    int choix;
    afffichage_actions();
    do {
        std::cout << "Veuillez saisir le numero de l'action souhaite : ";
        std::cin >> choix;
    } while ((choix > 3) || (choix < 0));
    switch (choix) {
        case 1:
            show_system_infos();
            break;
        case 2:
            show_liste_proc();
            break;
        default:
            std::cout << "Merci d'avoir utilisé WMS" << std::endl;
    };

    
    
    return 0;
}