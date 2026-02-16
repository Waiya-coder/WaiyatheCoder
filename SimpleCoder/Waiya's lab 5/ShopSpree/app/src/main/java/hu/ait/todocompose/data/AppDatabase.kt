package hu.ait.todocompose.data

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase


@Database(entities = [TodoItem::class], version = 2, exportSchema = false) // Updated version to 2
abstract class AppDatabase : RoomDatabase() {
    abstract fun todoDao(): TodoDAO

    companion object {
        @Volatile
        private var INSTANCE: AppDatabase? = null

        fun getDatabase(context: Context): AppDatabase {
            return INSTANCE ?: synchronized(this) {
                Room.databaseBuilder(
                    context.applicationContext,
                    AppDatabase::class.java,
                    "todo_database.db"
                )
                    .fallbackToDestructiveMigration() // Only for development/testing
                    .build().also { INSTANCE = it }
            }
        }
    }
}
