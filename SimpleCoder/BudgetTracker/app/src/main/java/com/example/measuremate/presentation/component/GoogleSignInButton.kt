package com.example.measuremate.presentation.component

import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.Icon
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.measuremate.R
import com.example.measuremate.presentation.signin.SignInScreen
import com.example.measuremate.presentation.theme.MeasureMateTheme

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color


@Composable
fun GoogleSignInButton(
    modifier: Modifier = Modifier,
    loadingState: Boolean = false,
    enabled: Boolean = true,
    primaryText: String = "Sign in with Google",
    secondaryText: String = "Please wait...",
    onClick: () -> Unit
) {
    val  buttonTextState =  remember { mutableStateOf(primaryText) }
    val  buttonText = buttonTextState.value



    LaunchedEffect(key1 = loadingState) {
        buttonTextState.value = if (loadingState) secondaryText else primaryText
    }

    Button(
        modifier =modifier.fillMaxWidth().padding(16.dp),
        enabled = enabled && !loadingState,
        onClick = { onClick() }
    ) {
        Icon(
            painter = painterResource(id = R.drawable.ic_google_logo),
            contentDescription = "google icon",
            tint = Color.Unspecified
        )
        Spacer(modifier = Modifier.width(8.dp))

        Text(text = buttonText)
        if (loadingState) {
            Spacer(modifier = Modifier.width(8.dp))
            CircularProgressIndicator(
                modifier = Modifier.size(16.dp),
                strokeWidth = 2.dp,
                color = MaterialTheme.colorScheme.onPrimary
            )
        }
    }
}

@Preview
@Composable
private fun GoogleSignInButtonPreview() {
    MeasureMateTheme {
        GoogleSignInButton(
            loadingState = true,
            onClick = {}
        )
    }
}