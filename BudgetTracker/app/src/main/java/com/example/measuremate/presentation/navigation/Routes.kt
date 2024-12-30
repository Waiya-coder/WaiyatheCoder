package com.example.measuremate.presentation.navigation
//serializable ?
//passing data between screens
import kotlinx.serialization.Serializable

@Serializable
sealed class Routes {


    @Serializable
    data object SignInScreen : Routes()
    @Serializable
    data object DashboardScreen : Routes()
    @Serializable
    data object AddItemScreen : Routes()
    @Serializable
    data class DetailsScreen(val expenseId: String) : Routes()
}