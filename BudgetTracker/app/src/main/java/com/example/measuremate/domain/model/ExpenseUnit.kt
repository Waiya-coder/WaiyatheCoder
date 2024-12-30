package com.example.measuremate.domain.model




enum class ExpenseUnit(
    val code: String,
    val label: String
) {
    USD(code = "$", label = "$"),
    EUR(code = "EUR", label = "Euros"),
    GBP(code = "GBP", label = "British Pounds"),
    KES(code = "KES", label = "Kenyan Shillings"),
    INR(code = "INR", label = "Indian Rupees"),
    CAD(code = "CAD", label = "Canadian Dollars"),
    JPY(code = "JPY", label = "Japanese Yen"),
    AUD(code = "AUD", label = "Australian Dollars")
}
