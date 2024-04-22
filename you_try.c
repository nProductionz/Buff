#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRODUCTS 50
#define MAX_NAME_LEN 69
#define KEY 'K'

// ANSI Color Codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    char name[MAX_NAME_LEN];
    int quantity;
} Product;

Product inventory[MAX_PRODUCTS];
int numProducts = 0;

void xorEncryptDecrypt(char *data, size_t data_len, char key) {
    for (size_t i = 0; i < data_len; ++i) {
        data[i] ^= key;
    }
}

void addProduct(const char *name, int quantity) {
    if (numProducts < MAX_PRODUCTS) {
        strcpy(inventory[numProducts].name, name);
        inventory[numProducts].name[MAX_NAME_LEN - 1] = '\0';
        inventory[numProducts].quantity = quantity;
        numProducts++;
        printf("\n" ANSI_COLOR_GREEN "Product added successfully!\n\n" ANSI_COLOR_RESET);
    } else {
        printf("\n" ANSI_COLOR_RED "Inventory full!\n\n" ANSI_COLOR_RESET);
    }
}

void saveProducts(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror(ANSI_COLOR_RED "Failed to open file for writing" ANSI_COLOR_RESET);
        return;
    }

    for (int i = 0; i < numProducts; i++) {
        xorEncryptDecrypt(inventory[i].name, strlen(inventory[i].name), KEY);
        fwrite(&inventory[i], sizeof(Product), 1, file);
        xorEncryptDecrypt(inventory[i].name, strlen(inventory[i].name), KEY); // Re-encrypt (decrypt) for memory safety
    }

    fclose(file);
    printf(ANSI_COLOR_CYAN "Products saved successfully!\n\n" ANSI_COLOR_RESET);
}

void loadProducts(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror(ANSI_COLOR_RED "Failed to open file for reading" ANSI_COLOR_RESET);
        return;
    }

    Product buffer;
    while (fread(&buffer, sizeof(Product), 1, file) == 1) {
        xorEncryptDecrypt(buffer.name, strlen(buffer.name), KEY);
        addProduct(buffer.name, buffer.quantity);
    }

    fclose(file);
    printf(ANSI_COLOR_CYAN "\nProducts loaded successfully!\n\n" ANSI_COLOR_RESET);
}

void listAllProducts() {
    printf("\n" ANSI_COLOR_CYAN "Listing all products:\n\n" ANSI_COLOR_RESET);
    for (int i = 0; i < numProducts; i++) {
        printf(ANSI_COLOR_YELLOW "%d. Product Name: %s, Quantity: %d\n" ANSI_COLOR_RESET, i + 1, inventory[i].name, inventory[i].quantity);
    }
    printf("\n");
}

int main() {
    loadProducts("products.dat");

    int choice;
    char name[MAX_NAME_LEN];
    int quantity;

    while (1) {
        printf(ANSI_COLOR_MAGENTA "\n1. Add Product\n\n2. List All Products\n\n3. Exit\n\nEnter choice: " ANSI_COLOR_RESET);
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf(ANSI_COLOR_BLUE "\nEnter product name: " ANSI_COLOR_RESET);
            scanf("%99s", name);
            printf(ANSI_COLOR_BLUE "\nEnter quantity: " ANSI_COLOR_RESET);
            scanf("%d", &quantity);
            addProduct(name, quantity);
            saveProducts("products.dat");
            break;
        case 2:
            listAllProducts();
            break;
        case 3:
            printf(ANSI_COLOR_GREEN "\nExiting program...\n" ANSI_COLOR_RESET);
            return 0;
        default:
            printf(ANSI_COLOR_RED "\nInvalid choice!\n\n" ANSI_COLOR_RESET);
        }
    }

    return 0;
}
