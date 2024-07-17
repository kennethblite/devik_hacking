#include <iostream>
#include <cstring>
#include <iomanip>

// Base class for all metal items with a union
class Metal {
public:
    union {
        int Thickness;  // Used by Shield
        int Stab;       // Used by Spearhead
        void (*Slice)(); // Used by Sword
    };

    virtual void Use() = 0; // Pure virtual function
};

// Derived class for Sword
class Sword : public Metal {
public:
    Sword(void (*sliceFunc)()) {
        Slice = sliceFunc;
    }

    void Use() override {
	std::cout << "SWORD TIME";
        if (Slice) {
            Slice();
        } else {
            std::cerr << "Slice function not set!\n";
        }
    }
};

// Derived class for Shield
class Shield : public Metal {
public:
    Shield(int thickness) {
        Thickness = thickness;
    }

    void Use() override {
        std::cout << "Current shield thickness is: " << Thickness << std::endl;
        std::cout << "Enter new shield thickness: ";
        std::cin >> Thickness;
        std::cout << "Updated shield thickness is: " << Thickness << std::endl;
    }
};

// Derived class for Spearhead
class Spearhead : public Metal {
public:
    Spearhead(int stab) {
        Stab = stab;
    }

    void Use() override {
        std::cout << "Spearhead stab value is: " << Stab << std::endl;
    }
};

// Example Slice function
void exampleSlice() {
    std::cout << "Sword slicing action!\n";
}

// Function to demonstrate the use of different Metal objects
void processMetal(Metal* metal) {
    metal->Use();
}

int main() {
    Metal* metal;
    int choice;

    std::cout << "Choose a metal object to use:\n";
    std::cout << "1. Sword\n2. Shield\n3. Spearhead\n";
    std::cin >> choice;

    switch (choice) {
        case 1:
            metal = new Sword(exampleSlice);
            break;
        case 2:
            int thickness;
            std::cout << "Enter shield thickness: ";
            std::cin >> thickness;
            metal = new Shield(thickness);
            break;
        case 3:
            int stab;
            std::cout << "Enter spearhead stab value: ";
            std::cin >> stab;
            metal = new Spearhead(stab);
            break;
        default:
            std::cerr << "Invalid choice!\n";
            return 1;
    }

    processMetal(metal);

    // Demonstrating potential security issue by casting between types
    if (choice == 2) { // If the choice was Shield
        // Casting Shield to Sword
        Sword* swordFromShield = reinterpret_cast<Sword*>(metal);
        std::cout << "Using Shield cast as Sword (unsafe):\n";
        swordFromShield->Use(); // Potentially unsafe operation
    } else if (choice == 3) { // If the choice was Spearhead
        // Casting Spearhead to Sword
        Sword* swordFromSpearhead = reinterpret_cast<Sword*>(metal);
        std::cout << "Using Spearhead cast as Sword (unsafe):\n";
        swordFromSpearhead->Use(); // Potentially unsafe operation
    }

    // Clean up
    delete metal;
    return 0;
}

