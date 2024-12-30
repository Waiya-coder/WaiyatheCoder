package com.example.measuremate.domain.repository



import com.example.budgettracker.domain.model.Expense
import com.example.measuremate.domain.model.User
import kotlinx.coroutines.flow.Flow

interface DatabaseRepository {


    fun getSignedInUser(): Flow<User?>


    suspend fun addUser(): Result<Boolean>
    suspend fun addExpense(expense: Expense): Result<Boolean>
    suspend fun deleteExpense(expenseId: String): Result<Boolean>
    fun getAllExpenses(): Flow<List<Expense>>
    fun getExpense(expenseId: String): Flow<Expense?>
}
