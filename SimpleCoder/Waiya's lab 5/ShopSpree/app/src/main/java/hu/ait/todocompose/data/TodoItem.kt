package hu.ait.todocompose.data

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey
import java.io.Serializable

@Entity(tableName = "todotable")
data class TodoItem(
    @PrimaryKey(autoGenerate = true) val id: Int = 0,
    @ColumnInfo(name = "title") val title: String,
    @ColumnInfo(name = "description") val description: String,
    @ColumnInfo(name = "createDate") val createDate: String,
    @ColumnInfo(name = "category") val category: TodoCategory,
    @ColumnInfo(name = "estimatedPrice") val estimatedPrice: Double,
    @ColumnInfo(name = "isDone") var isDone: Boolean
) : Serializable
