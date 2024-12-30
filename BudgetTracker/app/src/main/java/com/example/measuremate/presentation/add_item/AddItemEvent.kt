package com.example.measuremate.presentation.add_item

import com.example.budgettracker.domain.model.Expense
//import com.example.measuremate.domain.model.Expense

sealed class AddItemEvent {
    data class OnTextFieldValueChange(val value: String): AddItemEvent()
    data class OnItemClick(val expense: Expense): AddItemEvent()
    data class OnItemIsActiveChange(val expense: Expense): AddItemEvent()
    data object OnAddItemDialogDismiss: AddItemEvent()
    data object UpsertItem: AddItemEvent()
}