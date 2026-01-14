#include <iostream>
#include "../include/informations_system.h"







void afffichage_actions () {
    std::cout << "0)_Help" << std::endl;
    std::cout << "1)_Informations du systeme" << std::endl;
}




int main() {
    int choix;
    afffichage_actions();
    std::cout << "Veuillez saisir le numéro de l'action souhaité : ";
    std::cin >> choix;
    show_system_infos();
}