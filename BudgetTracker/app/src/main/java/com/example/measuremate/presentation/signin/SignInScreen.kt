package com.example.measuremate.presentation.signin

import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues

import androidx.compose.foundation.layout.Row

import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.material3.windowsizeclass.WindowWidthSizeClass


import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue

import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext

import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontStyle

import androidx.compose.ui.tooling.preview.PreviewScreenSizes
import androidx.compose.ui.unit.dp
import com.example.measuremate.R
import com.example.measuremate.presentation.component.AnonymousSignInButton
import com.example.measuremate.presentation.component.GoogleSignInButton
import com.example.measuremate.presentation.component.MeasureMateDialog

import com.example.measuremate.presentation.theme.MeasureMateTheme
import com.example.measuremate.presentation.util.UiEvent
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flowOf

@Composable
fun SignInScreen(
    snackbarHostState: SnackbarHostState,
    windowSize: WindowWidthSizeClass,
    paddingValues: PaddingValues,
    state: SignInState,
    uiEvent: Flow<UiEvent>,
    onEvent: (SignInEvent) -> Unit
) {
    val context = LocalContext.current
    LaunchedEffect (key1 = uiEvent){
        uiEvent.collect{event ->
            when (event) { is UiEvent.Snackbar -> {
                snackbarHostState.showSnackbar(
                    message = event.message,
                    actionLabel = event.actionLabel)
            }

                UiEvent.HideBottomSheet -> TODO()
                UiEvent.Navigate -> TODO()
            }
    }}

    var isSignInAnonymousDialogOpen by rememberSaveable() { mutableStateOf(false) }
    MeasureMateDialog(
        isOpen =isSignInAnonymousDialogOpen,
        title = "Login anonymously?",
        body = {
            Text(
                text = "By logging in anonymously, you will not be able to synchronize the data " +
                        "across devices or after uninstalling the app. \nAre you sure you want to proceed?"
            )
        },
        onDialogDismiss = {isSignInAnonymousDialogOpen = false},
        onConfirmButtonClick = {
            onEvent(SignInEvent.SignInAnonymously)
            isSignInAnonymousDialogOpen = false
        }
    )
    when (windowSize) {
        WindowWidthSizeClass.Compact -> {
            Column(
                modifier = Modifier.fillMaxSize().padding(paddingValues),
                horizontalAlignment = Alignment.CenterHorizontally,
                verticalArrangement = Arrangement.Center
            ) {
                Image(
                    painter = painterResource(id = R.drawable.app_logo),
                    contentDescription = "App Logo",
                    modifier = Modifier
                        .size(120.dp)
                        .padding(bottom = 16.dp)
                )
                Spacer(modifier = Modifier.height(40.dp))
                Text(
                    text = "BudgetTracker",
                    style = MaterialTheme.typography.headlineLarge
                )
                Text(
                    text = "You are not Bill Gates, Yet",
                    style = MaterialTheme.typography.bodyLarge.copy(fontStyle = FontStyle.Italic),
                    modifier = Modifier.padding(bottom = 24.dp)
                )
                Spacer(modifier = Modifier.fillMaxHeight(fraction = 0.4f))
                GoogleSignInButton(
                    loadingState = state.isGoogleSignInButtonLoading,
                    enabled = !state.isGoogleSignInButtonLoading,
                    onClick = { onEvent(SignInEvent.SignInWithGoogle(context)) }
                )




                Spacer(modifier = Modifier.height(20.dp))
                AnonymousSignInButton(
                    loadingState = state.isAnonymousSignInButtonLoading,
                    enabled = !state.isAnonymousSignInButtonLoading && !state.isAnonymousSignInButtonLoading,
                    onClick = { onEvent(SignInEvent.SignInAnonymously) }
                )
            }
        }

        else -> {
            Row(
                modifier = Modifier.fillMaxSize(),
            ) {
                Column(
                    modifier = Modifier
                        .fillMaxHeight()
                        .weight(1f),
                    horizontalAlignment = Alignment.CenterHorizontally,
                    verticalArrangement = Arrangement.Center
                ) {
                    Image(
                        painter = painterResource(id = R.drawable.app_logo),
                        contentDescription = "App Logo",
                        modifier = Modifier.size(150.dp)
                    )
                    Spacer(modifier = Modifier.height(16.dp))
                    Text(
                        text = "Budget Tracker",
                        style = MaterialTheme.typography.headlineLarge
                    )
                    Text(
                        text = "You are not Bill Gates, Yet",
                        style = MaterialTheme.typography.bodyLarge.copy(fontStyle = FontStyle.Italic)
                    )
                }
                Column(
                    modifier = Modifier
                        .fillMaxHeight()
                        .weight(1f),
                    horizontalAlignment = Alignment.CenterHorizontally,
                    verticalArrangement = Arrangement.Center
                ) {
                    GoogleSignInButton(
                        loadingState = state.isGoogleSignInButtonLoading,
                        enabled = !state.isGoogleSignInButtonLoading,
                        onClick = { onEvent(SignInEvent.SignInWithGoogle(context)) }
                    )
                    Spacer(modifier = Modifier.height(20.dp))
                    AnonymousSignInButton(
                        loadingState = state.isAnonymousSignInButtonLoading,
                        enabled = !state.isAnonymousSignInButtonLoading && !state.isAnonymousSignInButtonLoading,
                        onClick = { onEvent(SignInEvent.SignInAnonymously)  }
                    )
                }
            }
        }
    }
}


@PreviewScreenSizes
@Composable
private fun SignInScreenPreview() {
    MeasureMateTheme {
        SignInScreen(
            windowSize = WindowWidthSizeClass.Medium,
            paddingValues = PaddingValues(0.dp),
            state = SignInState(),
            onEvent = {},
            snackbarHostState = remember{SnackbarHostState()},
            uiEvent = flowOf()

        )
    }
}