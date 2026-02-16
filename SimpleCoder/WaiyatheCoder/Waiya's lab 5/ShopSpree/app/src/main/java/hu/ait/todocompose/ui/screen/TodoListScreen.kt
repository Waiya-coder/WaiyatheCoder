package hu.ait.todocompose.ui.screen

import android.annotation.SuppressLint
import androidx.compose.animation.animateContentSize
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowBack
import androidx.compose.material.icons.filled.AddCircle
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material.icons.filled.ArrowDropDown
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material.icons.filled.Edit
import androidx.compose.material.icons.filled.Info
import androidx.compose.material.icons.filled.KeyboardArrowDown
import androidx.compose.material.icons.filled.KeyboardArrowUp
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.runtime.livedata.observeAsState
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.style.TextDecoration
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.compose.ui.window.Dialog
import androidx.hilt.navigation.compose.hiltViewModel
import hu.ait.todocompose.R
import hu.ait.todocompose.data.TodoCategory
import hu.ait.todocompose.data.TodoItem
import kotlinx.coroutines.launch

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun TodoListScreen(
    modifier: Modifier = Modifier,
    viewModel: TodoViewModel = hiltViewModel(),
    onNavigateToSummary: (String) -> Unit,
    onViewItemDetails: (String) -> Unit
) {
    val coroutineScope = rememberCoroutineScope()
    val todoList by viewModel.todoList.observeAsState(emptyList()) // observe the LiveData

    var showAddDialog by remember { mutableStateOf(false) }
    var todoToEdit: TodoItem? by remember { mutableStateOf(null) }

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("SHOPPING LIST") },
                actions = {
                    IconButton(onClick = { showAddDialog = true }) {
                        Icon(Icons.Default.AddCircle, contentDescription = "Add")
                    }
                    IconButton(onClick = { viewModel.clearAllTodos() }) {
                        Icon(Icons.Default.Delete, contentDescription = "Delete all")
                    }
                    IconButton(onClick = {
                        coroutineScope.launch {
                            val allTodo = viewModel.getAllTodoNum()
                            onNavigateToSummary(allTodo.toString())
                        }
                    }) {
                        Icon(Icons.Default.Info, contentDescription = "Info")
                    }
                }
            )
        },
        modifier = modifier.fillMaxSize()
    ) { innerPadding ->
        Column(modifier = Modifier.padding(innerPadding)) {
            if (todoList.isEmpty()) {
                Text(
                    text = "No shopping today ? ",
                    modifier = Modifier.fillMaxSize().wrapContentSize(Alignment.Center)
                )
            } else {
                LazyColumn {
                    items(todoList) { todoItem ->
                        TodoCard(
                            todoItem = todoItem,
                            onTodoDelete = { viewModel.removeTodoItem(it) },
                            onTodoChecked = { item, checked -> viewModel.changeTodoState(item, checked) },

                            onTodoEdit = { item ->
                                todoToEdit = item
                                showAddDialog = true
                            },
                            onViewDetails = { onViewItemDetails(todoItem.id.toString()) }

                        )
                    }
                }
            }
        }
    }

    if (showAddDialog) {
        TodoDialog(
            viewModel = viewModel,
            todoToEdit = todoToEdit,
            onCancel = {
                showAddDialog = false
                todoToEdit = null
            }
        )
    }
}

@Composable
fun TodoDialog(
    viewModel: TodoViewModel,
    todoToEdit: TodoItem? = null,
    onCancel: () -> Unit
) {
    var todoTitle by remember { mutableStateOf(todoToEdit?.title ?: "") }
    var todoPrice by remember { mutableStateOf(todoToEdit?.estimatedPrice?.toString() ?: "") }
    var todoDesc by remember { mutableStateOf(todoToEdit?.description ?: "") }
    var selectedCategory by remember { mutableStateOf(todoToEdit?.category ?: TodoCategory.OTHER) }
    var expanded by remember { mutableStateOf(false) }
    var bought by remember { mutableStateOf(todoToEdit?.isDone ?: false) }
    var showError by remember { mutableStateOf(false) }

    Dialog(onDismissRequest = { onCancel() }) {
        Surface(
            modifier = Modifier.fillMaxWidth().wrapContentHeight(),
            shape = RoundedCornerShape(6.dp)
        ) {
            Column(modifier = Modifier.padding(15.dp)) {
                Text(text = if (todoToEdit == null) "New Todo" else "Edit Todo", style = MaterialTheme.typography.titleMedium)

                OutlinedTextField(
                    value = todoTitle,
                    onValueChange = { todoTitle = it },
                    label = { Text("Item name") },
                    isError = showError && todoTitle.isEmpty()
                )

                OutlinedTextField(
                    value = todoPrice,
                    onValueChange = { todoPrice = it },
                    label = { Text("Price") },
                    isError = showError && todoPrice.isEmpty()
                )

                OutlinedTextField(
                    value = todoDesc,
                    onValueChange = { todoDesc = it },
                    label = { Text("Description") },
                    isError = showError && todoDesc.isEmpty()
                )

                OutlinedTextField(
                    value = selectedCategory.name,
                    onValueChange = {},
                    label = { Text("Category") },
                    readOnly = true,
                    trailingIcon = { IconButton(onClick = { expanded = !expanded }) { Icon(Icons.Default.ArrowDropDown, contentDescription = "Select Category") } },
                    isError = showError && selectedCategory == TodoCategory.OTHER
                )

                DropdownMenu(expanded = expanded, onDismissRequest = { expanded = false }) {
                    TodoCategory.entries.forEach { category -> // Replaced values() with entries
                        DropdownMenuItem(
                            text = { Text(category.displayName) },
                            onClick = {
                                selectedCategory = category
                                expanded = false
                            }
                        )
                    }
                }


                Row(verticalAlignment = Alignment.CenterVertically) {
                    Checkbox(checked = bought, onCheckedChange = { bought = it })
                    Text("Bought")
                }

                if (showError) {
                    Text(
                        text = "Please fill out all fields and select a category.",
                        color = MaterialTheme.colorScheme.error,
                        modifier = Modifier.padding(top = 8.dp)
                    )
                }

                Row(modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.End) {
                    TextButton(onClick = {
                        val price = todoPrice.toDoubleOrNull() ?: 0.0

                        // Check if all required fields are filled out
                        if (todoTitle.isEmpty() || todoDesc.isEmpty() || todoPrice.isEmpty() || selectedCategory == TodoCategory.OTHER) {
                            showError = true
                        } else {
                            showError = false
                            if (todoToEdit == null) {
                                viewModel.addTodoList(
                                    TodoItem(
                                        title = todoTitle,
                                        description = todoDesc,
                                        createDate = System.currentTimeMillis().toString(),
                                        category = selectedCategory,
                                        estimatedPrice = price,
                                        isDone = bought
                                    )
                                )
                            } else {
                                viewModel.editTodoItem(
                                    todoToEdit.copy(
                                        title = todoTitle,
                                        description = todoDesc,
                                        category = selectedCategory,
                                        estimatedPrice = price,
                                        isDone = bought
                                    )
                                )
                            }
                            onCancel()
                        }
                    }) {
                        Text(if (todoToEdit == null) "Add Todo" else "Save")
                    }
                }
            }
        }
    }
}

@Composable
fun TodoCard(
    todoItem: TodoItem,
    onTodoDelete: (TodoItem) -> Unit,
    onTodoChecked: (TodoItem, Boolean) -> Unit,
    onTodoEdit: (TodoItem) -> Unit,
    onViewDetails: () -> Unit
) {
    Card(
        colors = CardDefaults.cardColors(containerColor = Color(0xFFE1F5E6)),
        shape = RoundedCornerShape(16.dp),
        elevation = CardDefaults.cardElevation(defaultElevation = 8.dp),
        modifier = Modifier
            .padding(8.dp)
            .fillMaxWidth()
    ) {
        var expanded by remember { mutableStateOf(false) }
        var todoChecked by remember { mutableStateOf(todoItem.isDone) }

        // Load category icon based on the selected category
        val categoryIcon = when (todoItem.category) {
            TodoCategory.FOOD -> painterResource(R.drawable.food)
            TodoCategory.ELECTRONIC -> painterResource(R.drawable.electronics)
            TodoCategory.BOOK -> painterResource(R.drawable.books)
            TodoCategory.OTHER -> painterResource(R.drawable.other)
        }

        Column(modifier = Modifier.padding(16.dp).animateContentSize()) {
            Row(
                verticalAlignment = Alignment.CenterVertically,
                modifier = Modifier.fillMaxWidth()
            ) {
                Icon(
                    painter = categoryIcon,
                    contentDescription = "Category Icon",
                    modifier = Modifier.size(48.dp).padding(end = 16.dp)
                )

                Column(modifier = Modifier.weight(1f)) {
                    Text(
                        text = todoItem.title,
                        textDecoration = if (todoItem.isDone) TextDecoration.LineThrough else TextDecoration.None,
                        style = MaterialTheme.typography.bodyLarge
                    )
                    Text(text = "${todoItem.estimatedPrice} $", fontSize = 12.sp)
                    Text(text = todoItem.description, fontSize = 12.sp)
                }

                // Align delete, edit, and expand buttons in a row at the top-right
                Row(
                    verticalAlignment = Alignment.CenterVertically,
                    horizontalArrangement = Arrangement.End
                ) {
                    IconButton(onClick = { onTodoEdit(todoItem) }) {
                        Icon(
                            imageVector = Icons.Default.Edit,
                            contentDescription = "Edit",
                            tint = Color.Gray
                        )
                    }
                    IconButton(onClick = { onTodoDelete(todoItem) }) {
                        Icon(
                            imageVector = Icons.Default.Delete,
                            contentDescription = "Delete",
                            tint = Color.Red
                        )
                    }
                    IconButton(onClick = { expanded = !expanded }) {
                        Icon(
                            imageVector = if (expanded) Icons.Default.KeyboardArrowUp else Icons.Default.KeyboardArrowDown,
                            contentDescription = if (expanded) "Less" else "More"
                        )
                    }
                }
            }

            // Details and action buttons
            if (expanded) {
                Row(verticalAlignment = Alignment.CenterVertically, modifier = Modifier.padding(top = 8.dp)) {
                    Checkbox(
                        checked = todoChecked,
                        onCheckedChange = {
                            todoChecked = it
                            onTodoChecked(todoItem, todoChecked)
                        }
                    )
                    Text("Bought")

                    Spacer(modifier = Modifier.width(8.dp))

                    Icon(
                        imageVector = Icons.Default.Delete,
                        contentDescription = "Delete",
                        tint = Color.Red,
                        modifier = Modifier.clickable { onTodoDelete(todoItem) }
                    )

                    Spacer(modifier = Modifier.width(8.dp))

                    Icon(
                        imageVector = Icons.Default.Edit,
                        contentDescription = "Edit",
                        tint = Color.Gray,
                        modifier = Modifier.clickable { onTodoEdit(todoItem) }
                    )

                    Spacer(modifier = Modifier.width(8.dp))

                    TextButton(onClick = { onViewDetails() }) { // or onViewDetails(todoItem.id) if it takes an Int
                        Text("View Details")
                    }
                }

                // Optional: Show category icon or other details
                Image(
                    painter = categoryIcon,
                    contentDescription = "Category Icon",
                    modifier = Modifier.size(48.dp),
                    contentScale = ContentScale.Fit
                )
            }
        }
    }
}


@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ItemDetailsScreen(
    todoItem: TodoItem,
    onBack: () -> Unit
) {
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text(text = "Item Details") },
                navigationIcon = {
                    IconButton(onClick = { onBack() }) {
                        Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "Back")
                    }
                }
            )
        }
    ) {
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp),
            verticalArrangement = Arrangement.Top,
            horizontalAlignment = Alignment.Start
        ) {
            Text(text = "Title: ${todoItem.title}", style = MaterialTheme.typography.titleLarge)
            Text(text = "Price: ${todoItem.estimatedPrice} $", style = MaterialTheme.typography.bodyLarge)
            Text(text = "Description: ${todoItem.description}", style = MaterialTheme.typography.bodyLarge)
            Text(text = "Category: ${todoItem.category.displayName}", style = MaterialTheme.typography.bodyLarge)
            Text(text = "Bought: ${if (todoItem.isDone) "Yes" else "No"}", style = MaterialTheme.typography.bodyLarge)
        }
    }
}
