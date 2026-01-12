#pragma once
#include <string>
#include <Windows.h>
#include <psapi.h>


std::string architecture_processeur (SYSTEM_INFO info_sys);

DWORD taille_page_memoire(SYSTEM_INFO info_sys);
DWORD nbre_processeur_grpe(SYSTEM_INFO info_sys);
std::string type_processeur(SYSTEM_INFO info_sys);

DWORD memoire_phys_utilise (MEMORYSTATUSEX mem_pc);
DWORDLONG memoire_phy_max (const MEMORYSTATUSEX & mem_pc);