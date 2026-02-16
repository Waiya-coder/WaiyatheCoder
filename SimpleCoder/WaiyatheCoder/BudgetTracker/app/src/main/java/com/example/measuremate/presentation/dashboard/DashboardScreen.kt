package com.example.measuremate.presentation.dashboard

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.WindowInsets
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowForward
import androidx.compose.material.icons.filled.Add
import androidx.compose.material3.Card
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.material3.rememberModalBottomSheetState
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.tooling.preview.PreviewScreenSizes
import androidx.compose.ui.unit.dp
import com.example.budgettracker.domain.model.Expense
import com.example.budgettracker.domain.model.predefinedExpenses
import com.example.measuremate.domain.model.User
import com.example.measuremate.presentation.component.MeasureMateDialog
import com.example.measuremate.presentation.component.ProfileBottomSheet
import com.example.measuremate.presentation.component.ProfilePicPlaceholder
import com.example.measuremate.presentation.theme.MeasureMateTheme
import com.example.measuremate.presentation.util.UiEvent
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flowOf

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DashboardScreen(
    snackbarHostState: SnackbarHostState,
    paddingValues: PaddingValues,
    onFabClicked: () -> Unit,
    uiEvent: Flow<UiEvent>,
    state: DashboardState,
    onEvent: (DashboardEvent) -> Unit,
    onItemCardClicked: (String) -> Unit,)
{
    val context = LocalContext.current
    LaunchedEffect (key1 = uiEvent){
        uiEvent.collect{event ->
            when (event) { is UiEvent.Snackbar -> {
                snackbarHostState.showSnackbar(
                    message = event.message,
                    actionLabel = event.actionLabel)
            }

                UiEvent.HideBottomSheet -> TODO()
                UiEvent.Navigate -> TODO()
            }
        }}


    var isProfileBottomSheetOpen by remember { mutableStateOf(false) }
    var isSignOutDialogOpen by rememberSaveable { mutableStateOf(false) }
    val bottomSheetState = rememberModalBottomSheetState()

    val isUserAnonymous = state.user?.isAnonymous ?: true
    ProfileBottomSheet(
        isOpen = isProfileBottomSheetOpen,
        sheetState = bottomSheetState,
        user = state.user,
        buttonLoadingState = if (isUserAnonymous) state.isSignInButtonLoading else state.isSignOutButtonLoading,
        buttonPrimaryText = if (isUserAnonymous) "Sign in with Google" else "Sign out with Google",
        onBottomSheetDismiss = { isProfileBottomSheetOpen = false },
        onGoogleButtonClick = {
            if (isUserAnonymous) onEvent(DashboardEvent.AnonymousUserSignInWithGoogle(context))
            else isSignOutDialogOpen = true
        }
    )


    MeasureMateDialog(
        isOpen = isSignOutDialogOpen,
        title = "Sign Out",
        body = { Text(text = "Are you sure, you want to sign Out?") },
        onDialogDismiss = { isSignOutDialogOpen = false },
        onConfirmButtonClick = {
            onEvent(DashboardEvent.SignOut)
            isSignOutDialogOpen = false
        }
    )
    Box (
        modifier = Modifier.fillMaxSize().padding(paddingValues)
    )
        {
            Column(
                modifier = Modifier.fillMaxSize()

            ){
                DashboardTopBar(
                    profilePicUrl = state.user?.profilePictureUrl,
                    onProfilePicClick = {isProfileBottomSheetOpen = true}

                )
                LazyVerticalGrid(
                    modifier = Modifier.fillMaxSize(),
                    columns = GridCells.Adaptive(minSize =300.dp),
                    contentPadding = PaddingValues(16.dp, top = 8.dp, 16.dp, 16.dp),
                    horizontalArrangement = androidx.compose.foundation.layout.Arrangement.spacedBy(32.dp),
                    verticalArrangement = androidx.compose.foundation.layout.Arrangement.spacedBy(12.dp)
                )

                {
                    items(predefinedExpenses){
                            expense ->
                        ItemCard(
                            expense = expense,
                            onItemCardClicked =  onItemCardClicked
                        )
                    }
                }


            }
            FloatingActionButton(
                modifier = Modifier
                    .align(Alignment.BottomEnd)
                    .padding(24.dp),
                onClick = {  onFabClicked()}
            ) {
                Icon(imageVector = Icons.Default.Add, contentDescription = "Add Icon")
            }
        }


}



@OptIn(ExperimentalMaterial3Api::class)
@Composable
private fun DashboardTopBar(
    modifier: Modifier = Modifier,
    profilePicUrl: String?,
    onProfilePicClick: () -> Unit

)
{
    TopAppBar(
        modifier = modifier,
        windowInsets = WindowInsets(0, 0, 0, 0),
        title = {

            Text(text = "BudgetTracker")
        },

        actions = {

            IconButton(onClick = { onProfilePicClick() }) {
                ProfilePicPlaceholder(
                    placeholderSize = 30.dp,
                    borderWidth = 1.dp,
                    profilePictureUrl = profilePicUrl,
                    padding = 2.dp
                )
            }



}
    )
}

@Composable
private fun ItemCard(
    modifier: Modifier = Modifier,
    expense: Expense,
    onItemCardClicked: (String) -> Unit
) {
    Card(
        modifier = modifier,
        onClick = { expense.expenseId?.let { onItemCardClicked(it) } }
    ) {
        Row(
            modifier = Modifier.padding(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Text(
                modifier = Modifier.weight(8f),
                text = expense.title,
                style = MaterialTheme.typography.bodyLarge,
                maxLines = 1,
                overflow = TextOverflow.Ellipsis
            )
            Spacer(modifier = Modifier.weight(1f))
            Text(
                text = "${expense.amount ?: ""} ${expense.currency}",
                style = MaterialTheme.typography.bodyLarge
            )
            Spacer(modifier = Modifier.width(10.dp))
            Box(
                modifier = Modifier
                    .clip(CircleShape)
                    .background(MaterialTheme.colorScheme.surface)
                    .padding(4.dp)
            ) {
                Icon(
                    imageVector = Icons.AutoMirrored.Filled.ArrowForward,
                    contentDescription = "Show Details",
                    modifier = Modifier.size(15.dp)
                )
            }
        }
    }
}

@PreviewScreenSizes
@Composable
private fun DashboardScreenPreview() {
    MeasureMateTheme {
        DashboardScreen(
            onItemCardClicked = {},
            uiEvent = flowOf(),

            onFabClicked = {},
            paddingValues = PaddingValues(0.dp),
            snackbarHostState = remember{SnackbarHostState()},
                onEvent = {},
                state = DashboardState(),


        )
    }}


