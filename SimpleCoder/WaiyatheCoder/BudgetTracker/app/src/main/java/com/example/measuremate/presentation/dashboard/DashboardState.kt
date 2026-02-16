package com.example.measuremate.presentation.dashboard

import com.example.budgettracker.domain.model.Expense
import com.example.measuremate.domain.model.User

data class DashboardState(
    val user: User? = null,
    val expenses: List<Expense> = emptyList(),
    val isSignOutButtonLoading: Boolean = false,
    val isSignInButtonLoading: Boolean = false,
)