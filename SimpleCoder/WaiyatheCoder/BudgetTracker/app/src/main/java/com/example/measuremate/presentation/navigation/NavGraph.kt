package com.example.measuremate.presentation.navigation


//import androidx.hilt.navigation.compose.hiltViewModel
//import androidx.navigation.NavHostController
//import androidx.navigation.compose.NavHost
//import androidx.navigation.compose.composable
//import com.example.measuremate.presentation.add_item.AddItemViewModel
//import com.example.measuremate.presentation.dashboard.DashboardViewModel
//import com.example.measuremate.presentation.details.DetailsViewModel
import androidx.compose.animation.AnimatedContentTransitionScope
import androidx.compose.animation.core.tween
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.windowsizeclass.WindowWidthSizeClass
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.ViewModel
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.toRoute
import com.example.measuremate.presentation.add_item.AddItemScreen
import com.example.measuremate.presentation.add_item.AddItemState
import com.example.measuremate.presentation.dashboard.DashboardScreen
import com.example.measuremate.presentation.dashboard.DashboardState
import com.example.measuremate.presentation.dashboard.DashboardViewModel
import com.example.measuremate.presentation.details.DetailsScreen
import com.example.measuremate.presentation.signin.SignInScreen
import com.example.measuremate.presentation.signin.SignInViewModel


//import com.example.measuremate.presentation.signin.SignInViewModel
//import com.example.measuremate.presentation.util.UiEvent

@Composable
fun NavGraph(
    snackbarHostState: SnackbarHostState,
    navController: NavHostController,
    signInViewModel: SignInViewModel,

    windowSize: WindowWidthSizeClass,
    paddingValues: PaddingValues

    ) {
    NavHost(
        navController = navController,
        startDestination = Routes.DashboardScreen
    ) {

        composable<Routes.SignInScreen> {

            val state by signInViewModel.state.collectAsStateWithLifecycle()
SignInScreen(
    paddingValues = paddingValues,
    windowSize = windowSize,
    snackbarHostState = snackbarHostState,
    state = state,
    onEvent = signInViewModel::onEvent,
    uiEvent = signInViewModel.uiEvent
)

        }

        composable<Routes.DashboardScreen> {
            val viewModel = hiltViewModel<DashboardViewModel>()
            val state by viewModel.state.collectAsStateWithLifecycle()
            DashboardScreen(
                onFabClicked = { navController.navigate(Routes.AddItemScreen) },
                paddingValues = paddingValues,
                snackbarHostState = snackbarHostState,
                uiEvent = signInViewModel.uiEvent,
                state = state,
                onEvent = viewModel::onEvent,
                onItemCardClicked = { expenseId ->
                    navController.navigate(Routes.DetailsScreen(expenseId))

                }
                    )
        }

        composable<Routes.AddItemScreen>(
            enterTransition = {
                slideIntoContainer(
                    animationSpec = tween(durationMillis = 500),
                    towards = AnimatedContentTransitionScope.SlideDirection.Start
                )
            },
            exitTransition = {
                slideOutOfContainer(
                    animationSpec = tween(durationMillis = 500),
                    towards = AnimatedContentTransitionScope.SlideDirection.End
                )
            }
        )

         {
            AddItemScreen(
                state = AddItemState(),
                onEvent = {},
                onBackIconClick = {navController.navigateUp()},
                paddingValues = paddingValues,
                snackbarHostState = snackbarHostState
            )
        }


        composable<Routes.DetailsScreen>(
            enterTransition = {
                slideIntoContainer(
                    animationSpec = tween(durationMillis = 500),
                    towards = AnimatedContentTransitionScope.SlideDirection.Start
                )
            },
            exitTransition = {
                slideOutOfContainer(
                    animationSpec = tween(durationMillis = 500),
                    towards = AnimatedContentTransitionScope.SlideDirection.End
                )
            }
        ) { navBackStackEntry ->
            val expenseId = navBackStackEntry.toRoute<Routes.DetailsScreen>().expenseId
            DetailsScreen(
                expenseId = expenseId,
                windowSize = windowSize,
                paddingValues = paddingValues,
                snackbarHostState = remember{SnackbarHostState()},
                onBackIconClick = {navController.navigateUp()}
            )
        }}}
