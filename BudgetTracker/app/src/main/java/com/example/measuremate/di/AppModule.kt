package com.example.measuremate.di
import android.content.Context
import androidx.credentials.CredentialManager
import com.example.measuremate.data.repository.AuthRepositoryImpl
import com.example.measuremate.data.repository.DatabaseRepositoryImpl
import com.example.measuremate.domain.repository.AuthRepository
import com.example.measuremate.domain.repository.DatabaseRepository
import com.google.firebase.Firebase
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.auth
import com.google.firebase.firestore.FirebaseFirestore
import com.google.firebase.firestore.firestore
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.components.ViewModelComponent
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.android.scopes.ViewModelScoped


@Module
@InstallIn(ViewModelComponent::class)
object AppModule {

    @Provides
    @ViewModelScoped
    fun provideFirebaseAuth(): FirebaseAuth {
        return Firebase.auth
    }

    @Provides
    @ViewModelScoped
    fun provideFirebaseFirestore(): FirebaseFirestore {
        return Firebase.firestore
    }

    @Provides
    @ViewModelScoped
    fun provideCredentialManager(
        @ApplicationContext context: Context
    ): CredentialManager {
        return CredentialManager.create(context)
    }



    @Provides
    @ViewModelScoped
    fun provideAuthRepository(
        firebaseAuth: FirebaseAuth,
        credentialManager: CredentialManager
    ): AuthRepository {
        return AuthRepositoryImpl(firebaseAuth, credentialManager)
    }

    @Provides
    @ViewModelScoped
    fun provideDatabaseRepository(
        firebaseAuth: FirebaseAuth,
        firestore: FirebaseFirestore
    ): DatabaseRepository {
        return DatabaseRepositoryImpl(firebaseAuth,firestore)
    }
}

