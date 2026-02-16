package hu.ait.todocompose.navigation


sealed class MainNavigation(val route: String) {
    object TodoListScreen : MainNavigation("todolist")

    object SummaryScreen : MainNavigation(
        "summaryscreen?all={all}") {
        fun createRoute(all: Int) : String {
            return "summaryscreen?all=$all"
        }
    }

}