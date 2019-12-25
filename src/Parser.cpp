/*************************************************************************
                           Parser  -  description
                             -------------------
    début                : 2019-11-19
    copyright            : (C) 2019 FADILI Zineb & FORLER Corentin
    e-mail               : zineb.fadili@insa-lyon.fr
                           corentin.forler@insa-lyon.fr
*************************************************************************/

// - Réalisation de la classe <Parser> (fichier Parser.cpp) --
//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <fstream>
#include <iostream>
using namespace std;

//------------------------------------------------------ Include personnel
#include "CompoundTrip.h"
#include "ListOfTrips.h"
#include "Parser.h"
#include "SimpleTrip.h"

//----------------------------------------------------------------- PUBLIC
ListOfTrips *Parser::Parse(ifstream &input)
{
    ListOfTrips *parseResults = new ListOfTrips();

    unsigned int lineIndex = 0;
    string line;

    while (std::getline(input, line))
    {
        lineIndex += 1;

        if (line == ">")
        {
            string startCity;
            string endCity;
            string mode;

            std::getline(input, startCity);
            std::getline(input, endCity);
            std::getline(input, mode);
            lineIndex += 3;

            Trip *trip = new SimpleTrip(startCity.c_str(), endCity.c_str(), mode.c_str());
            parseResults->Add(trip);
        }
        else if (line == "@")
        {
            unsigned int nSubTrips;
            input >> nSubTrips;
            std::getline(input, line); // on ignore les caractères restants sur la ligne

            if (line != "")
            {
                cerr << "Erreur de lecture: la ligne " << lineIndex << " doit ne contenir qu'un nombre.";
                input.close();
                return nullptr;
            }

            ListOfTrips *subtrips = new ListOfTrips(nSubTrips);

            string city1;
            string city2;
            string mode;

            std::getline(input, city1);
            for (unsigned int i = 0; i < nSubTrips; i++)
            {
                std::getline(input, city2);
                std::getline(input, mode);

                SimpleTrip *theSubtrip = new SimpleTrip(city1.c_str(), city2.c_str(), mode.c_str());
                subtrips->Add(theSubtrip);

                city1 = city2;
            }

            Trip *trip = new CompoundTrip(subtrips);
            parseResults->Add(trip);

            lineIndex += 1;
            // On lit <nombre> éléments.
        }
        else
        {
            cerr << "Erreur de lecture: caractère invalide à la ligne " << lineIndex << "." << endl;
            cerr << "  ‘@’ ou ‘>’ attendu." << endl;
            input.close();
            return nullptr;
        }
    }

    return parseResults;
}

//----------------------------------------------------- Méthodes publiques

//-------------------------------------------- Constructeurs - destructeur

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
