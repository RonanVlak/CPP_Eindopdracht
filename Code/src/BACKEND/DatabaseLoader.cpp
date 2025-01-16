#include "DatabaseLoader.h"
#include <iostream>
#include <stdexcept>

// RAII wrapper for sqlite3 database connection
class SQLiteDB {
public:
    SQLiteDB(const char* filename) {
        if (sqlite3_open(filename, &db) != SQLITE_OK) {
            throw std::runtime_error("Could not open database");
        }
    }

    ~SQLiteDB() {
        if (db) {
            sqlite3_close(db);
        }
    }

    sqlite3* get() const { return db; }

private:
    sqlite3* db = nullptr;
};

// RAII wrapper for sqlite3 statement
class SQLiteStmt {
public:
    SQLiteStmt(sqlite3* db, const char* sql) {
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement");
        }
    }

    ~SQLiteStmt() {
        if (stmt) {
            sqlite3_finalize(stmt);
        }
    }

    sqlite3_stmt* get() const { return stmt; }

private:
    sqlite3_stmt* stmt = nullptr;
};

CustomVector<Locatie*> DatabaseLoader::laadLocaties(const char* databaseBestand, int& count) {
    CustomVector<Locatie*> locaties;

    try {
        SQLiteDB db(databaseBestand);
        const char* sql = "SELECT naam, beschrijving FROM Locaties";
        SQLiteStmt stmt(db.get(), sql);

        LocatieFactory locatieFactory;

        while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
            const char* naam = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
            const char* beschrijving = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));

            Locatie* locatie = locatieFactory.CreateLocatie(count, naam, beschrijving, beschrijving, "", "", "", "", "", "", "", databaseBestand);
            locaties.push_back(locatie);
            count++;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        count = 0;
    }

    return locaties;
}

CustomVector<Vijand*> DatabaseLoader::laadVijanden(const char* databaseBestand, int& count) {
    CustomVector<Vijand*> vijandenVector;

    try {
        SQLiteDB db(databaseBestand);
        const char* sql = "SELECT naam, omschrijving, minimumobjecten, maximumobjecten, levenspunten, aanvalskans, minimumschade, maximumschade FROM Vijanden";
        SQLiteStmt stmt(db.get(), sql);

        VijandFactory factory;

        while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
            const char* naam = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
            const char* beschrijving = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
            int minimumobjecten = sqlite3_column_int(stmt.get(), 2);
            int maximumobjecten = sqlite3_column_int(stmt.get(), 3);
            int levenspunten = sqlite3_column_int(stmt.get(), 4);
            int aanvalskans = sqlite3_column_int(stmt.get(), 5);
            int minimumschade = sqlite3_column_int(stmt.get(), 6);
            int maximumschade = sqlite3_column_int(stmt.get(), 7);

            Vijand* vijand = factory.CreateVijand(naam, beschrijving, minimumobjecten, maximumobjecten, levenspunten, aanvalskans, minimumschade, maximumschade);

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
                CustomVector<Spelobject*> allSpelobjecten = laadSpelobjecten(databaseBestand, dummyCount);
                if (randomIndex < allSpelobjecten.size()) {
                    vijand->voegSpelobjectToe(allSpelobjecten[randomIndex]);
                    allSpelobjecten[randomIndex] = nullptr;
                }
                for (auto& obj : allSpelobjecten) {
                    delete obj;
                }
            }

            vijandenVector.push_back(vijand);
        }

        count = vijandenVector.size();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        count = 0;
    }

    return vijandenVector;
}

CustomVector<Spelobject*> DatabaseLoader::laadSpelobjecten(const char* databaseBestand, int& count) {
    CustomVector<Spelobject*> spelobjectenVector;

    try {
        SQLiteDB db(databaseBestand);
        const char* sql = "SELECT naam, omschrijving, type, minimumwaarde, maximumwaarde, bescherming FROM Objecten";
        SQLiteStmt stmt(db.get(), sql);

        SpelobjectFactory factory;

        while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
            const char* naam = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
            const char* beschrijving = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
            const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 2));
            int minimumwaarde = sqlite3_column_int(stmt.get(), 3);
            int maximumwaarde = sqlite3_column_int(stmt.get(), 4);
            int bescherming = sqlite3_column_int(stmt.get(), 5);

            Spelobject* object = factory.CreateSpelobject(naam, beschrijving, type, minimumwaarde, maximumwaarde, bescherming);
            spelobjectenVector.push_back(object);
        }

        count = spelobjectenVector.size();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        count = 0;
    }

    return spelobjectenVector;
}