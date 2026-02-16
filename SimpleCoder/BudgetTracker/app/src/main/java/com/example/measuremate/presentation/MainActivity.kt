package com.example.measuremate.presentation

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.material3.Scaffold
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.material3.windowsizeclass.ExperimentalMaterial3WindowSizeClassApi
import androidx.compose.material3.windowsizeclass.calculateWindowSizeClass
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.navigation.compose.rememberNavController
import com.example.measuremate.data.util.Constants.APP_LOG
import com.example.measuremate.domain.model.AuthStatus
import com.example.measuremate.presentation.navigation.NavGraph
import com.example.measuremate.presentation.navigation.Routes
import com.example.measuremate.presentation.signin.SignInViewModel
import com.example.measuremate.presentation.theme.MeasureMateTheme

import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class MainActivity : ComponentActivity() {

    @OptIn(ExperimentalMaterial3WindowSizeClassApi::class)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        installSplashScreen()
        enableEdgeToEdge()
        setContent {
            MeasureMateTheme {
                val windowSizeClass = calculateWindowSizeClass(activity = this)
                val navController = rememberNavController()
                val signInViewModel: SignInViewModel = hiltViewModel()

                val authStatus by signInViewModel.authStatus.collectAsStateWithLifecycle()
                var previousAuthStatus by rememberSaveable { mutableStateOf<AuthStatus?>(null) }
                Log.d(APP_LOG, "authStatus: $authStatus")
                LaunchedEffect(key1 = authStatus) {
                    if (previousAuthStatus != authStatus) {
                        when (authStatus) {
                            AuthStatus.AUTHORISED -> {
                                navController.navigate(Routes.DashboardScreen) { popUpTo(0) }
                                Log.d(APP_LOG, "authStatus: AUTHORISED; navigateToDashboard")
                            }

                            AuthStatus.UNAUTHORISED -> {
                                navController.navigate(Routes.SignInScreen) { popUpTo(0) }
                                Log.d(APP_LOG, "authStatus: UNAUTHORISED; navigateToSignIn")
                            }

                            AuthStatus.LOADING -> {}
                            AuthStatus.UNAUTHENTICATED -> TODO()
                            AuthStatus.AUTHENTICATED -> TODO()
                            AuthStatus.ERROR -> TODO()
                        }
                        previousAuthStatus = authStatus
                    }
                }

                val snackbarHostState = remember { SnackbarHostState() }
                Scaffold(
                    snackbarHost = { SnackbarHost(hostState = snackbarHostState) }
                ) { paddingValues ->
                    NavGraph(
                        navController = navController,
                        snackbarHostState = snackbarHostState,
                        windowSize = windowSizeClass.widthSizeClass,
                        paddingValues = paddingValues,
                        signInViewModel = signInViewModel
                    )
                }
            }
        }
    }
}

//package com.example.measuremate.presentation
//
//import android.os.Bundle
//import android.util.Log
//import androidx.activity.ComponentActivity
//import androidx.activity.compose.setContent
//import androidx.activity.enableEdgeToEdge
//import androidx.compose.material3.Scaffold
//import androidx.compose.material3.SnackbarHost
//import androidx.compose.material3.SnackbarHostState
//import androidx.compose.material3.windowsizeclass.ExperimentalMaterial3WindowSizeClassApi
//import androidx.compose.material3.windowsizeclass.calculateWindowSizeClass
//import androidx.compose.runtime.LaunchedEffect
//import androidx.compose.runtime.remember
//import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
//import androidx.hilt.navigation.compose.hiltViewModel
//import androidx.navigation.compose.rememberNavController
//import com.example.measuremate.data.util.Constants.APP_LOG
//import com.example.measuremate.presentation.navigation.NavGraph
//import com.example.measuremate.presentation.navigation.Routes
//import com.example.measuremate.presentation.signin.SignInViewModel
//import com.example.measuremate.presentation.theme.MeasureMateTheme
//import dagger.hilt.android.AndroidEntryPoint
//
//@AndroidEntryPoint
//class MainActivity : ComponentActivity() {
//
//    @OptIn(ExperimentalMaterial3WindowSizeClassApi::class)
//    override fun onCreate(savedInstanceState: Bundle?) {
//        super.onCreate(savedInstanceState)
//        installSplashScreen()
//        enableEdgeToEdge()
//        setContent {
//            MeasureMateTheme {
//                val windowSizeClass = calculateWindowSizeClass(activity = this)
//                val navController = rememberNavController()
//                val signInViewModel: SignInViewModel = hiltViewModel()
//                Log.d(APP_LOG, "Skipping sign-in, navigating directly to DashboardScreen")
//
//                // Navigate directly to DashboardScreen on app launch
//                LaunchedEffect(Unit) {
//                    navController.navigate(Routes.DashboardScreen) {
//                        popUpTo(0)
//                    }
//                }
//
//                val snackbarHostState = remember { SnackbarHostState() }
//                Scaffold(
//                    snackbarHost = { SnackbarHost(hostState = snackbarHostState) }
//                ) { paddingValues ->
//                    NavGraph(
//                        navController = navController,
//                        snackbarHostState = snackbarHostState,
//                        windowSize = windowSizeClass.widthSizeClass,
//                        paddingValues = paddingValues,
//                        signInViewModel = signInViewModel
//                    )
//                }
//            }
//        }
//    }
//}
