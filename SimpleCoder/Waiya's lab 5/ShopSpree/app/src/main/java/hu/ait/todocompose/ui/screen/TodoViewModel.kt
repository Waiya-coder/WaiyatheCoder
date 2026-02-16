package hu.ait.todocompose.ui.screen

import androidx.compose.runtime.mutableStateListOf
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import dagger.hilt.android.lifecycle.HiltViewModel
import hu.ait.todocompose.R
import hu.ait.todocompose.data.TodoDAO
import hu.ait.todocompose.data.TodoItem
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class TodoViewModel @Inject constructor(
    val todoDAO: TodoDAO
) : ViewModel() {
    private val _todoList = MutableLiveData<List<TodoItem>>()
    val todoList: LiveData<List<TodoItem>> = _todoList

    fun getTodoItemById(id: Int): TodoItem? {
        return todoList.value?.find { it.id == id }
    }



    fun getAllToDoList(): Flow<List<TodoItem>> {
        return todoDAO.getAllTodos()
    }

    fun addTodoList(todoItem: TodoItem) {
        val updatedList = _todoList.value.orEmpty().toMutableList().apply {
            add(todoItem)
        }
        _todoList.value = updatedList
    }

    fun removeTodoItem(todoItem: TodoItem) {
        val updatedList = _todoList.value.orEmpty().toMutableList().apply {
            remove(todoItem)
        }
        _todoList.value = updatedList
    }

    fun editTodoItem(editedTodo: TodoItem) {
        viewModelScope.launch(Dispatchers.IO) {
            todoDAO.update(editedTodo)
        }
    }

    fun changeTodoState(todoItem: TodoItem, value: Boolean) {
        val updatedTodo = todoItem.copy()
        updatedTodo.isDone = value
        viewModelScope.launch(Dispatchers.IO) {
            todoDAO.update(updatedTodo)
        }
    }

    fun clearAllTodos() {
        _todoList.value = emptyList() // Set the list to an empty list to clear all items
    }

    suspend fun getAllTodoNum(): Int {
        return todoDAO.getTodosNum()
    }

//    suspend fun getImportantTodoNum(): Int {
//        return todoDAO.getImportantTodosNum()
//    }
}

