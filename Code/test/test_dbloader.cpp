#include <gtest/gtest.h>
#include "DatabaseLoader.h"
#include "Locatie.h"
#include "Vijand.h"
#include "Spelobject.h"
#include "sqlite3.h"
#include <cstdio>

class DatabaseLoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        databaseBestand = "test_database.db";
        setupTestDatabase();
    }

    void TearDown() override {
        std::remove(databaseBestand.c_str());
    }

    void setupTestDatabase() {
        sqlite3* db;
        sqlite3_open(databaseBestand.c_str(), &db);

        const char* createLocatiesTable = R"(
            CREATE TABLE Locaties (
                id INTEGER PRIMARY KEY,
                naam TEXT NOT NULL,
                beschrijving TEXT
            );
            INSERT INTO Locaties (naam, beschrijving) VALUES ('Start', 'Startlocatie');
        )";

        const char* createVijandenTable = R"(
            CREATE TABLE Vijanden (
                id INTEGER PRIMARY KEY,
                naam TEXT NOT NULL,
                omschrijving TEXT,
                minimumobjecten INTEGER,
                maximumobjecten INTEGER,
                levenspunten INTEGER,
                aanvalskans INTEGER,
                minimumschade INTEGER,
                maximumschade INTEGER
            );
            INSERT INTO Vijanden (naam, omschrijving, minimumobjecten, maximumobjecten, levenspunten, aanvalskans, minimumschade, maximumschade) VALUES ('Goblin', 'Een gevaarlijke goblin', 0, 2, 10, 50, 5, 10);
        )";

        const char* createObjectenTable = R"(
            CREATE TABLE Objecten (
                id INTEGER PRIMARY KEY,
                naam TEXT NOT NULL,
                omschrijving TEXT,
                type TEXT,
                minimumwaarde INTEGER,
                maximumwaarde INTEGER,
                bescherming INTEGER
            );
            INSERT INTO Objecten (naam, omschrijving, type, minimumwaarde, maximumwaarde, bescherming) VALUES ('Zwaard', 'Een scherp zwaard', 'wapen', 10, 20, 0);
        )";

        const char* createLeaderboardTable = R"(
            CREATE TABLE Leaderboard (
                id INTEGER PRIMARY KEY,
                naam TEXT NOT NULL,
                goudstukken INTEGER
            );
        )";

        sqlite3_exec(db, createLocatiesTable, nullptr, nullptr, nullptr);
        sqlite3_exec(db, createVijandenTable, nullptr, nullptr, nullptr);
        sqlite3_exec(db, createObjectenTable, nullptr, nullptr, nullptr);
        sqlite3_exec(db, createLeaderboardTable, nullptr, nullptr, nullptr);

        sqlite3_close(db);
    }

    std::string databaseBestand;
    DatabaseLoader dbLoader;
};

TEST_F(DatabaseLoaderTest, LaadLocaties) {
    int count = 0;
    CustomVector<Locatie*> locaties = dbLoader.laadLocaties(databaseBestand.c_str(), count);
    EXPECT_EQ(count, 1);
    ASSERT_EQ(locaties.size(), 1);
    EXPECT_STREQ(locaties[0]->getNaam(), "Start");
    EXPECT_STREQ(locaties[0]->getAlgemeneOmschrijving(), "Startlocatie");

    for (auto locatie : locaties) {
        delete locatie;
    }
}

TEST_F(DatabaseLoaderTest, LaadVijanden) {
    int count = 0;
    CustomVector<Vijand*> vijanden = dbLoader.laadVijanden(databaseBestand.c_str(), count);
    EXPECT_EQ(count, 1);
    ASSERT_EQ(vijanden.size(), 1);
    EXPECT_STREQ(vijanden[0]->getNaam(), "Goblin");
    EXPECT_STREQ(vijanden[0]->getBeschrijving(), "Een gevaarlijke goblin");

    for (auto vijand : vijanden) {
        delete vijand;
    }
}

TEST_F(DatabaseLoaderTest, LaadSpelobjecten) {
    int count = 0;
    CustomVector<Spelobject*> spelobjecten = dbLoader.laadSpelobjecten(databaseBestand.c_str(), count);
    EXPECT_EQ(count, 1);
    ASSERT_EQ(spelobjecten.size(), 1);
    EXPECT_STREQ(spelobjecten[0]->getNaam(), "Zwaard");
    EXPECT_STREQ(spelobjecten[0]->getBeschrijving(), "Een scherp zwaard");

    for (auto spelobject : spelobjecten) {
        delete spelobject;
    }
}

TEST_F(DatabaseLoaderTest, VoegLeaderboardToe) {
    dbLoader.voegLeaderboardToe(databaseBestand.c_str(), "Speler1", 100);

    sqlite3* db;
    sqlite3_open(databaseBestand.c_str(), &db);

    const char* sql = "SELECT naam, goudstukken FROM Leaderboard WHERE naam = 'Speler1'";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    ASSERT_EQ(sqlite3_step(stmt), SQLITE_ROW);
    EXPECT_STREQ(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)), "Speler1");
    EXPECT_EQ(sqlite3_column_int(stmt, 1), 100);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

TEST_F(DatabaseLoaderTest, PrintLeaderboard) {
    dbLoader.voegLeaderboardToe(databaseBestand.c_str(), "Speler1", 100);
    dbLoader.voegLeaderboardToe(databaseBestand.c_str(), "Speler2", 200);

    testing::internal::CaptureStdout();
    dbLoader.printLeaderboard(databaseBestand.c_str());
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Speler2 - 200 goudstukken"), std::string::npos);
    EXPECT_NE(output.find("Speler1 - 100 goudstukken"), std::string::npos);
}