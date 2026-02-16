package com.example.budgettracker.domain.model

import com.example.measuremate.domain.model.ExpenseUnit


data class Expense(
    val title: String,
    val amount: Float,
    val category: String,
    val isActive: Boolean,
    val date: String,
    val currency: ExpenseUnit, // Added ExpenseUnit here
    val expenseId: String? = null
)

val predefinedExpenses: List<Expense> = listOf(
    Expense(
        title = "Groceries",
        amount = 120.50f,
        category = "Food",
        date = "2024-12-01",
        isActive = true,
        currency = ExpenseUnit.USD, // Currency in US Dollars,
        expenseId = "541"
    // Currency in US Dollars
    ),
    Expense(
        title = "Electricity Bill",
        amount = 75.00f,
        category = "Utilities",
        date = "2024-12-01",
        isActive = true,
        currency = ExpenseUnit.USD,// Currency in US Dollars
                expenseId = "541"
    ),
    Expense(
        title = "Gym Membership",
        amount = 50.00f,
        category = "Fitness",
        date = "2024-12-01",
        isActive = true,
        expenseId = "521",
        currency = ExpenseUnit.USD // Currency in US Dollars
    ),
    Expense(
        title = "Netflix Subscription",
        amount = 15.99f,
        category = "Entertainment",
        date = "2024-12-01",
        isActive = true,
        expenseId = "531",
        currency = ExpenseUnit.USD// Currency in US Dollars
    ),
    Expense(
        title = "Bus Ticket",
        amount = 2.50f,
        category = "Transport",
        date = "2024-12-01",
        isActive = true,
        expenseId = "511",
        currency = ExpenseUnit.USD// Currency in US Dollars
    )
)
