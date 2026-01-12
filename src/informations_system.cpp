#include "../include/informations_system.h"
#include <iostream>
#include <string>
#include <fstream>

#include <Windows.h>
#include <psapi.h>





std::string architecture_processeur (SYSTEM_INFO info_sys) {
    switch (info_sys.wProcessorArchitecture) {
    case 9:
        return "PROCESSOR_ARCHITECTURE_AMD64";
    
    case 5:
        return "PROCESSOR_ARCHITECTURE_ARM";
    
    case 12:
        return "PROCESSOR_ARCHITECTURE_ARM64";

    case 6:
        return "PROCESSOR_ARCHITECTURE_IA64";

    case 0:
        return "PROCESSOR_ARCHITECTURE_INTEL";

    case 0xffff:
        return "PROCESSEUR NON RECONNU";

    default:
        return "PROCESSEUR NON RECONNU";
    };

}


DWORD taille_page_memoire (SYSTEM_INFO info_sys) {
    return info_sys.dwPageSize;
}


//Atention cette focntion retourne le nbre de processeur logique dans le groupe actuel contenant le programme.
DWORD nbre_processeur_grpe (SYSTEM_INFO info_sys) {
    return info_sys.dwNumberOfProcessors;
}

std::string type_processeur(SYSTEM_INFO info_sys) {
    DWORD type_proc = info_sys.dwProcessorType;
    switch (type_proc) {
        case 386:
            return "PROCESSOR_AMD_X8664 ";
        
        case 2200:
            return "PROCESSOR_INTEL_IA64 ";

        case 486:
            return "PROCESSOR_INTEL_486 ";

        case 586:
            return "PROCESSOR_INTEL_PENTIUM ";
        
        case 8664:
            return "PROCESSOR_AMD_X8664 ";
        
        default:
            return "Type inconnu";
    };

}



DWORD memoire_phys_utilise (MEMORYSTATUSEX mem_pc) {
    return mem_pc.dwMemoryLoad;
}

DWORDLONG memoire_phy_max (const MEMORYSTATUSEX & mem_pc) {
    return mem_pc.ullTotalPhys;
}


void creation_fic (std::string nom_fic) {
    std::ofstream outfile (nom_fic);
    outfile << "==============================================================================================================" << std::endl;
    outfile << "================================ INFORMATIONS RELATIVE AU SYSTEME ============================================" << std::endl;
    outfile << "==============================================================================================================" << std::endl;
    outfile.close();
}

void choix_export() {
    char choix;
    std::string fic_infosys = "data_info_system.txt";
    std::cout << "Exporter les données dans un fichier texte ?" << std::endl;
    do {
        std::cout << "Veuillez saisir 'o' ou 'n' (o=oui   n=non) : ";
        std::cin >> choix;
    } while ((choix != 'o') and (choix != 'n') and (choix != 'O') and (choix != 'N'));
    if ((choix == 'o') or (choix == 'n')) creation_fic(fic_infosys);
}


int main() {
    SYSTEM_INFO info_sys;
    GetSystemInfo(&info_sys);
    MEMORYSTATUSEX info_mem;       
    info_mem.dwLength = sizeof(info_mem);       //on doit tjrs initialiser le champ dwLength de la structure MEMORYSTATUSEX avant l’appel
    if (!GlobalMemoryStatusEx(&info_mem)) {
        std::cerr << "Erreur lors de la récupération de la mémoire" << std::endl;
        return 1;
    };
    
    std::string processeur_machine = architecture_processeur(info_sys);
    DWORD taille_page = taille_page_memoire(info_sys);    //en octets
    DWORD nbre_proc_grpe = nbre_processeur_grpe(info_sys);
    std::string type_proc = type_processeur(info_sys);
    DWORD percent_mem_phys = memoire_phys_utilise(info_mem);
    double mem_phys_totale = static_cast<double>(info_mem.ullTotalPhys) / (1024.0*1024.0*1024.0);

    std::cout << "==============================================================================================================" << std::endl;
    std::cout << "================================ INFORMATIONS RELATIVE AU SYSTEME ============================================" << std::endl;
    std::cout << "==============================================================================================================" << std::endl;
    std::cout << "Processeur de la machine : " << processeur_machine << std::endl;
    std::cout << "Taille page memoire : " << taille_page << " octets." << std::endl;
    std::cout << "Nombre de processeurs logiques dans le groupe actuel : " << nbre_proc_grpe << std::endl;
    std::cout << "Type du processeur :" << type_proc << std::endl;
    std::cout << std::endl;

    std::cout << "==============================================================================================================" << std::endl;
    std::cout << "================================ INFORMATIONS RELATIVE A LA MEMOIRE ==========================================" << std::endl;
    std::cout << "==============================================================================================================" << std::endl;
    std::cout << "Pourcentage de memoire physique utilise : " << percent_mem_phys << "%." << std::endl;
    std::cout << "Quantite de la RAM utilisable : " << mem_phys_totale << " GO." << std::endl;

    choix_export();

    /*
    std::ofstream outfile ("test.txt");
    outfile << "my text here!" << std::endl;
    outfile.close();
    */
    return 0;
}





