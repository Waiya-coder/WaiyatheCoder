package hu.ait.todocompose

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import dagger.hilt.android.AndroidEntryPoint
import hu.ait.todocompose.navigation.MainNavigation
import hu.ait.todocompose.ui.screen.ItemDetailsScreen
import hu.ait.todocompose.ui.screen.SplashScreen
import hu.ait.todocompose.ui.screen.SummaryScreen
import hu.ait.todocompose.ui.screen.TodoListScreen
import hu.ait.todocompose.ui.screen.TodoViewModel
import hu.ait.todocompose.ui.theme.TodoComposeTheme

@AndroidEntryPoint
class MainActivity : ComponentActivity() {
    @OptIn(ExperimentalMaterial3Api::class)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            TodoComposeTheme {
                val navController = rememberNavController()
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    val viewModel: TodoViewModel = hiltViewModel()
                    TodoAppNavHost(
                        navController = navController,
                        modifier = Modifier.padding(innerPadding),
                        viewModel = viewModel
                        
                    )
                }
            }
        }
    }
}

@Composable
fun TodoAppNavHost(
    modifier: Modifier = Modifier,
    navController: NavHostController = rememberNavController(),
    startDestination: String = "splash",
    viewModel: TodoViewModel
) {
    NavHost(
        navController = navController,
        startDestination = startDestination,
        modifier = modifier
    ) {
        composable("splash") {
            SplashScreen(navController = navController)
        }

        composable(MainNavigation.TodoListScreen.route) {
            TodoListScreen(
                onNavigateToSummary = { all ->
                    val allInt = all.toIntOrNull() // Convert to Int if possible
                    if (allInt != null) {
                        navController.navigate(MainNavigation.SummaryScreen.createRoute(allInt))
                    } else {
                        // Handle the error if conversion fails (e.g., log error or show a message)
                    }
                }
                ,
                onViewItemDetails = { todoItemId ->
                    navController.navigate("item_details/$todoItemId")
                },
                viewModel = viewModel
            )
        }

        composable("item_details/{todoItemId}") { backStackEntry ->
            val todoItemId = backStackEntry.arguments?.getString("todoItemId")?.toIntOrNull()
            val todoItem =
                todoItemId?.let { viewModel.getTodoItemById(it) } // Ensure `getTodoItemById` is updated to accept an Int
            if (todoItem != null) {
                ItemDetailsScreen(
                    todoItem = todoItem,
                    onBack = { navController.popBackStack() }
                )
            }
        }

        composable(MainNavigation.SummaryScreen.route) {
            SummaryScreen()
        }
    }}