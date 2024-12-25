#include "DatabaseLoader.h"
#include <iostream>

CustomVector<CustomUniquePtr<Locatie>> DatabaseLoader::laadLocaties(const char* databaseBestand, int& count) {
    CustomVector<CustomUniquePtr<Locatie>> locaties;
    sqlite3* db;
    sqlite3_stmt* stmt;

    if (sqlite3_open(databaseBestand, &db) != SQLITE_OK) {
        std::cerr << "Kan database niet openen: " << sqlite3_errmsg(db) << std::endl;
        return locaties;
    }

    const char* sql = "SELECT naam, beschrijving FROM Locaties";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Kan SQL statement niet voorbereiden: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return locaties;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    LocatieFactory locatieFactory;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* naam = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* beschrijving = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        Locatie* locatie = locatieFactory.CreateLocatie(count, naam, beschrijving, beschrijving, "", "", "", "", "", "", "", databaseBestand);

        locaties.push_back(CustomUniquePtr<Locatie>(locatie));
        count++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return locaties;
}

CustomVector<CustomUniquePtr<Vijand>> DatabaseLoader::laadVijanden(const char* databaseBestand, int& count) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT naam, omschrijving, minimumobjecten, maximumobjecten, levenspunten, aanvalskans, minimumschade, maximumschade FROM Vijanden";

    CustomVector<CustomUniquePtr<Vijand>> vijandenVector;

    if (sqlite3_open(databaseBestand, &db) != SQLITE_OK) {
        std::cerr << "Could not open database: " << sqlite3_errmsg(db) << std::endl;
        count = 0;
        return vijandenVector;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        count = 0;
        sqlite3_close(db);
        return vijandenVector;
    }

    VijandFactory factory;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* naam = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* beschrijving = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int minimumobjecten = sqlite3_column_int(stmt, 2);
        int maximumobjecten = sqlite3_column_int(stmt, 3);
        int levenspunten = sqlite3_column_int(stmt, 4);
        int aanvalskans = sqlite3_column_int(stmt, 5);
        int minimumschade = sqlite3_column_int(stmt, 6);
        int maximumschade = sqlite3_column_int(stmt, 7);

        auto vijand = factory.CreateVijand(naam, beschrijving, minimumobjecten, maximumobjecten, levenspunten, aanvalskans, minimumschade, maximumschade);
        
        // Load random Spelobjecten for the Vijand
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(minimumobjecten, maximumobjecten);
        int aantalSpelobjecten = dis(gen);

        int totalObjects = 34; // Total number of objects in the database
        std::uniform_int_distribution<> objectDis(0, totalObjects - 1);

        for (int i = 0; i < aantalSpelobjecten; ++i) {
            int randomIndex = objectDis(gen);
            int dummyCount = 0;
            CustomVector<CustomUniquePtr<Spelobject>> allSpelobjecten = laadSpelobjecten(databaseBestand, dummyCount);
            if (randomIndex < allSpelobjecten.size()) {
                vijand->voegSpelobjectToe(std::move(allSpelobjecten[randomIndex]));
            }
        }

        vijandenVector.push_back(std::move(vijand));

    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    count = vijandenVector.size();

    return vijandenVector;
}

CustomVector<CustomUniquePtr<Spelobject>> DatabaseLoader::laadSpelobjecten(const char* databaseBestand, int& count) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT naam, omschrijving, type, minimumwaarde, maximumwaarde, bescherming FROM Objecten";

    CustomVector<CustomUniquePtr<Spelobject>> spelobjectenVector;

    if (sqlite3_open(databaseBestand, &db) != SQLITE_OK) {
        std::cerr << "Could not open database: " << sqlite3_errmsg(db) << std::endl;
        count = 0;
        return spelobjectenVector;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        count = 0;
        sqlite3_close(db);
        return spelobjectenVector;
    }

    SpelobjectFactory factory;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* naam = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* beschrijving = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int minimumwaarde = sqlite3_column_int(stmt, 3);
        int maximumwaarde = sqlite3_column_int(stmt, 4);
        int bescherming = sqlite3_column_int(stmt, 5);

        auto object = factory.CreateSpelobject(naam, beschrijving, type, minimumwaarde, maximumwaarde, bescherming);
        spelobjectenVector.push_back(std::move(object));
    }

    count = spelobjectenVector.size();
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return spelobjectenVector;
}