//
//  main.cpp
//  incomeTax
//
//  Created by George Taylor on 25/01/2018.
//  Copyright © 2018 George Taylor. All rights reserved.
//

#include <iostream>
#include <iomanip>


//constants for tax rates
const double basicRate = 0.20;
const double higherRate = 0.40;
const double addRate = 0.45;

//constants for tax bands
const double personalAllowance = 11509.00;
const double personalAllowanceBand = 11500.00;
const double basicMax = 45000.00;
const double higherMax = 150000.00;
const double higherMin = 45001.00;
const double loseNPA = 100000.00;
const double higherNPA = 124000.00;
const double higherBand = 45000.00;

//constants for NI
const double lel = 113.00;
const double niMin = 157.00;
const double niHigher = 866.00;
const double niRate = 0.12;
const double niHigherRate = 0.02;

double calcNI(double income) {
    double niCont;
    double weeklyIncome = income/52;
    if (weeklyIncome < lel) {
        niCont = 0;
        std::cout << "You don't pay National Insurance, and you also aren't eligable for the benefits of it \n";
    } else if ((weeklyIncome < niMin) && weeklyIncome > lel) {
        niCont = 0;
        std::cout << "You don't pay National Insurance, but you are legible to recieve the benefits of the scheme \n";
    } else if (weeklyIncome >= niMin && weeklyIncome < niHigher) {
        niCont = (weeklyIncome * niRate);
    } else if (weeklyIncome >= niHigher) {
        niCont = ((niHigher-niMin)*niRate) + ((weeklyIncome-niHigher)*niHigherRate);
    } else {
        niCont = 0;
    }
    return niCont;
}


double getIncome() {
    double income;
    do {
        std::cout << "Enter your pre-tax income: ";
        std::cin >> income;
        if (income <0){
            std::cout << "income must be more than nothing lol" << std::endl;
        }
    } while (income < 0);
    return income;
}


//this function takes the taxable variable in as a parameter and returns the amount of tax paid for a basic tax payer.
double calcBasicTax(double taxableIncome){
    double basic;
    std::cout << "You have to pay the Basic Tax rate\n";
    basic = taxableIncome;
    double taxAmount = basic * basicRate;
    return taxAmount;
}

//this function takes the taxable variable in as a parameter and returns the amount of tax paid for a higher tax payer.
double calcHigherTax(double income) {
    double basic, higher;
    std::cout << "You have to pay the Higher Tax rate \n";
    basic = basicMax - personalAllowanceBand;
    std::cout << "basic: " <<basic;
    higher = income - basicMax + personalAllowanceBand;
    std::cout << "higher: " <<higher;
    double taxAmount = (basic * basicRate) + (higher * higherRate);
    return taxAmount;
}

//this function takes the taxable variable in as a parameter and returns the amount of tax paid for an additional tax payer.
double calcAddTax(double income) {
    double basic, higher,add;
    std::cout << "You have to pay the Additional Tax rate \n";
    basic = basicMax - personalAllowanceBand;
    std::cout << "basic: " <<basic;
    higher = higherMax - higherBand + personalAllowanceBand;
    std::cout << "higher: " <<higher;
    add = income - higherMax + personalAllowance;
    std::cout << "add: " <<add;
    double taxAmount = (basic * basicRate) + (higher * higherRate) + (add * addRate);
    return taxAmount;
}


// this function is actually useless.
double noTax(double income) {
    return income;
}


int main(int argc, const char * argv[]) {
    
    //two decimal points
    std::cout << std::fixed;
    std::cout << std::setprecision(2);
    
    //initialise double variables
    double income;
    double taxAmount = 0.00;
    double postTaxIncome = 0.00;
    
    //get income from the getIncome Function and assign it to the income variable
    income = getIncome();
    //calculate the taxble portion of your income
    double taxableIncome = income - personalAllowance;
    
    double NI = calcNI(income);
    
    // No Tax
    if (income < personalAllowance) {
        std::cout << "You dont have to pay any income tax so your take home income is £" << income << std::endl;
        
        //Basic Tax 11500 - 45001 bracket
    } else if (income > personalAllowanceBand && income < higherMin) {
        taxAmount = calcBasicTax(taxableIncome);
        postTaxIncome = income - taxAmount -  (NI*52);
        
        //Higher Tax 45001 - 100000 bracket
    } else if (income > higherMin && income <= loseNPA) {
        taxAmount = calcHigherTax(taxableIncome);
        postTaxIncome = income - taxAmount - (NI*52);
        
        // 100000 - 123000 bracket
    } else if (income > loseNPA && income < higherNPA) {
        //calculate a reducing personal allowance
        double newPA = personalAllowance - ((income-loseNPA)/2);
        
        //disallow a negative personal allowance
        if (newPA < 0) {
            newPA =0;
        }
        
        //Test
        //std::cout << "new pa=" << newPA;
        
        double taxableIncome = income - newPA;
        taxAmount = calcHigherTax(taxableIncome);
        postTaxIncome = income - taxAmount - (NI*52);
        
        // 123000 - 15000 bracket
    } else if (income >= higherNPA && income < higherMax) {
        double taxableIncomeNoPA = taxableIncome + personalAllowance;
        taxAmount = calcHigherTax(taxableIncomeNoPA);
        postTaxIncome = income - taxAmount - (NI*52);
        
        //150000+ bracket
    }else if (income > higherNPA) {
        taxAmount = calcAddTax(taxableIncome);
        postTaxIncome = income - taxAmount - (NI*52);
    }
    
    //nice pretty sentences at the end
    std::cout << "Your income tax amount is: £" << taxAmount << std::endl << "Post income tax is £" << postTaxIncome +(NI*52) << "\nNI Contribution is £" << NI*52 << " which is equal to £" << NI << " per week \n" << "So, your take home pay (after all deductions) is: £" << postTaxIncome << std::endl;
    
    return 0;
}
