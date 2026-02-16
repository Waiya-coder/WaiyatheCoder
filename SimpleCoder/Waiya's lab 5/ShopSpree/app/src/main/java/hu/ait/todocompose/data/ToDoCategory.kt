package hu.ait.todocompose.data



import hu.ait.todocompose.R

enum class TodoCategory(val displayName: String, val iconRes: Int) {
    FOOD("Food", R.drawable.food),
    ELECTRONIC("Electronic", R.drawable.electronics),
    BOOK("Book", R.drawable.books),
    OTHER("Other", R.drawable.other)
}
