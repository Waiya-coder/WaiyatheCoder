package com.example.measuremate.data.repository

import com.google.firebase.auth.FirebaseAuth
import com.example.measuremate.domain.repository.DatabaseRepository
import kotlinx.coroutines.flow.Flow
import com.example.budgettracker.domain.model.Expense
import com.example.measuremate.data.mapper.UserDto
import com.example.measuremate.data.mapper.toUser
import com.example.measuremate.data.util.Constants.USER_COLLECTION
import com.example.measuremate.domain.model.User
import com.google.firebase.firestore.CollectionReference

import com.google.firebase.firestore.FirebaseFirestore
import com.google.firebase.firestore.Query
import com.google.firebase.firestore.snapshots
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.flow.flowOf
import kotlinx.coroutines.tasks.await


class DatabaseRepositoryImpl(
    private val firebaseAuth: FirebaseAuth,
    private val firebaseFirestore: FirebaseFirestore
) : DatabaseRepository {

    private fun userCollection(): CollectionReference {
        return firebaseFirestore
            .collection(USER_COLLECTION)
    }


    override suspend fun addUser(): Result<Boolean> {
        return try {
            val firebaseUser = firebaseAuth.currentUser
                ?: throw IllegalArgumentException("No current user logged in.")
            var user= User(
                name = "Anonymous",
                email = "anonymous@budgettracker.io",
                profilePictureUrl = "",
                isAnonymous = firebaseUser.isAnonymous,
                userId = firebaseUser.uid,

            )
            firebaseUser.providerData.forEach { profile ->
                user= user.copy(
                    name = profile.displayName ?: user.name,
                    email = profile.email ?: user.email,
                    profilePictureUrl = profile.photoUrl?.toString() ?: user.profilePictureUrl
                )
            }
            userCollection()
                .document(firebaseUser.uid)
                .set(user)
                .await()
            Result.success(value = true)
        } catch (e: Exception) {
            Result.failure(e)
        }
    }

    override fun getSignedInUser(): Flow<User?> {
        return flow {
            try {
                val userId = firebaseAuth.currentUser?.uid.orEmpty()
                userCollection()
                    .document(userId)
                    .snapshots()
                    .collect { snapshot ->
                        val userDto = snapshot.toObject(UserDto::class.java)
                        emit(userDto?.toUser())
                    }
            } catch (e: Exception) {
                throw e
            }
        }
    }

    override suspend fun addExpense(expense: Expense): Result<Boolean> {
        TODO("Not yet implemented")
    }

    override suspend fun deleteExpense(expenseId: String): Result<Boolean> {
        TODO("Not yet implemented")
    }

    override fun getAllExpenses(): Flow<List<Expense>> {
        // Implement logic using firebaseAuth or return a mock flow
        return flowOf(emptyList())
    }

    override fun getExpense(expenseId: String): Flow<Expense?> {
        TODO("Not yet implemented")
    }
}
