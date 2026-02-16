package com.example.measuremate.presentation.add_item

import com.example.budgettracker.domain.model.Expense

data class AddItemState(
    val textFieldValue: String = "",
    val selectedExpense: Expense? = null,
    val expenses: List<Expense> = emptyList()
)

