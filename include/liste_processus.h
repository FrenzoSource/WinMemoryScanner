#pragma once
#include <iostream>
#include <Windows.h>
#include <psapi.h>

HANDLE openProcess_pid();
void affichage_erreur(DWORD num_erreur);
int show_liste_proc();
std::string extraction_nom_exe(char * tampon, DWORD taille);