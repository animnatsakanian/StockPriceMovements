#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Creating nodes of linked lists
struct Node {
    double data;
    struct Node* next;
};
typedef struct Node Node;

// Creating a node
Node* createNode(double tvyal) {
    Node* NewNode = (Node*)malloc(sizeof(Node));
    NewNode->data = tvyal;
    NewNode->next = NULL;
    return NewNode;
}

// Append new node
void append(Node** head, double tvyal) {
    Node* new_node = createNode(tvyal);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node* last_node = *head;
    while (last_node->next != NULL) {
        last_node = last_node->next;
    }
    last_node->next = new_node;
}


Node* readDataFromFilePositive() {
    const char* filename = "./data.csv";
    Node* list = NULL;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file %s.\n", filename);
        return NULL;
    }

    double data;
    while (fscanf(file, "%lf", &data) != EOF) {
        if (data > 0) {
            append(&list, data);
        }
    }

    fclose(file);
    return list;
}

Node* readDataFromFileNegative() {
    const char* filename = "./data.csv";
    Node* list = NULL; // Arajin nodei hasce
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file %s.\n", filename);
        return NULL;
    }

    double data;
    while (fscanf(file, "%lf", &data) != EOF) {
        if (data < 0) {
            append(&list, data);
        }
    }

    fclose(file);
    return list;
}

//function to calculate means
double mean(Node* list) {
    double sum = 0;
    Node* lastNode = list;
    int i;
    for (i = 0; lastNode != NULL; i++) {
        sum += lastNode->data;
        lastNode = lastNode->next;
    }
    return sum / (i == 0 ? 1 : i);
}

// Function to calculate factorial
int factorial(int num) {
    if (num <= 1)
        return 1;
    else
        return num * factorial(num - 1);
}

int main() {
    double num;
    Node* positive = readDataFromFilePositive();
    Node* negative = readDataFromFileNegative();

    // Given data
    double stockprice;
    double intRate;
    int periods;
    printf("Please input the stock price\t");
    scanf("%lf", &stockprice);
    printf("Please input the interest rate\t");
    scanf("%lf", &intRate);
    printf("Please input the number of periods\t");
    scanf("%d", &periods);

    // Counting the means
    double meanPos = 1 + mean(positive) / 100;
    double meanNeg = 1 + mean(negative) / 100;
    double p = pow((1 + intRate), ((1.0 / periods) - meanNeg)) / (meanPos - meanNeg);


    //counting the stock prices within periods
    double* prices=(double*)malloc(sizeof(double)*(periods+1));
    double temp;


    for (int i=0; i<=periods; i++) {
        temp=stockprice*pow(meanPos, i)*pow(meanNeg, periods-i);
        prices[i]=temp;

    }
  
    //counting binom distribution
    double* binom=(double*)malloc(sizeof(double)*(periods-1));

    for(int i=0; i<=periods; i++) {
        binom[i] = (factorial(periods) / (factorial(i) * factorial(periods - i))* pow(p, i) * pow(1-p, periods-i));
    
    }
    
    double probofloss;
    
    for(int i=0; i<=periods; i++) {
        if(prices[i]<stockprice) {
            probofloss+=binom[i];
        }
    }

    double probofsuccess = 1-probofloss;

    //ASCII table
    printf("\n");
    printf("|Periods    |\t");
    for (int i=0; i<=periods; i++) {
        printf("%d\t|\t", i);
    }
    printf("\n");

    printf("|Stock Price|\t");
    for (int i=0; i<=periods; i++) {
        printf("%.2lf\t|\t", prices[i]);
    }
    printf("\n");

    printf("|Probability|\t");
    for (int i=0; i<=periods; i++) {
        printf("%.2lf\t|\t", binom[i]);
    }
    printf("\n");
    
    //Aditional information
    printf("\nYour probability of success is: %lf %c\n", probofsuccess*100, 37);
    printf("Your probability of loss is: %lf %c\n", probofloss*100, 37);
    printf("In each of periods the price may rise by %lf times or fall down by %lf times. ", meanPos, meanNeg);


    return 0;
}
