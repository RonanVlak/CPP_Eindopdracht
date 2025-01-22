#pragma once
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class Spelwereld;
class Locatie;
class WapenObject;
class Vijand;

class SpelwereldFacade {
public:
    SpelwereldFacade();
    ~SpelwereldFacade();

    // Prevent copying
    SpelwereldFacade(const SpelwereldFacade&) = delete;
    SpelwereldFacade& operator=(const SpelwereldFacade&) = delete;

    // Allow moving
    SpelwereldFacade(SpelwereldFacade&&) noexcept;
    SpelwereldFacade& operator=(SpelwereldFacade&&) noexcept;

    // Core functionality
    void generateRandomKerker(const std::string& databasePath);
    std::unique_ptr<WapenObject> getStartWapen(const std::string& databasePath);
    
    // Location management
    Locatie* getCurrentLocatie() const;
    void setCurrentLocatie(Locatie* locatie);
    Locatie* getLocatie(const std::string& naam);
    Locatie* getLocatieById(int id) const;
    Locatie* getLocatieByIndex(int index) const;
    void voegLocatieToe(std::unique_ptr<Locatie> locatie);
    std::vector<Locatie*> getAllLocaties() const;
    int getLocatiesCount() const;
    Spelwereld* getSpelwereld();

    // Game mechanics
    int getEnemiesDamage();
    void verplaatsVijanden();
    std::vector<Locatie*> getAdjacentLocations(Locatie* locatie);

    void clear();

private:
    // The wrapped Spelwereld instance
    std::unique_ptr<Spelwereld> mSpelwereld;
};