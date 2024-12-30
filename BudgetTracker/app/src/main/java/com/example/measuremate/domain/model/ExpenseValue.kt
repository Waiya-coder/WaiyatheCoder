package com.example.measuremate.domain.model

import java.time.LocalDate
data class ExpenseValue(
    val value: Float,
    val date: LocalDate,
    val expenseId: String? = null,
    val expenseValueId: String? = null,

)
