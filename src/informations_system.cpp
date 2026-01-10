#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <string>




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



DWORD memoire_phys_utilise (MEMORYSTATUSEX mem_pc) {
    return mem_pc.dwMemoryLoad;
}

DWORDLONG memoire_phy_max (const MEMORYSTATUSEX & mem_pc) {
    return mem_pc.ullTotalPhys;
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
    DWORD percent_mem_phys = memoire_phys_utilise(info_mem);
    //DWORDLONG mem_phys_totale = memoire_phy_max(info_mem) / (1024ULL * 1024ULL * 1024ULL);  
    double mem_phys_totale = static_cast<double>(info_mem.ullTotalPhys) / (1024.0*1024.0*1024.0);

    std::cout << "Processeur de la machine : " << processeur_machine << std::endl;
    std::cout << "Taille page memoire : " << taille_page << " octets." << std::endl;
    std::cout << "Nombre de processeurs logiques dans le groupe actuel : " << nbre_proc_grpe << std::endl;
    std::cout << "Pourcentage de memoire physique utilise : " << percent_mem_phys << "%." << std::endl;
    std::cout << "Quantite de la RAM utilisable : " << mem_phys_totale << " GO." << std::endl;

    return 0;
}





