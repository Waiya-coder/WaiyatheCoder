package com.example.measuremate.presentation.component

//import com.google.firebase.firestore.auth.User



import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.BottomSheetDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.material3.SheetState
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.measuremate.domain.model.User

//import com.example.measuremate.domain.model.User

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ProfileBottomSheet(
    modifier: Modifier = Modifier,
    isOpen: Boolean,
    sheetState: SheetState,

    user: User?,
    buttonLoadingState: Boolean,
    buttonPrimaryText: String,
    onGoogleButtonClick: () -> Unit,
    onBottomSheetDismiss: () -> Unit
) {
    if (isOpen) {
        ModalBottomSheet(
            modifier = modifier,
            sheetState = sheetState,
            onDismissRequest = { onBottomSheetDismiss() },
            dragHandle = {
                Column(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    BottomSheetDefaults.DragHandle()
                    Text(
                        text = "Profile",
                        style = MaterialTheme.typography.titleLarge
                    )
                    Spacer(modifier = Modifier.height(10.dp))
                    HorizontalDivider()
                }
            }
        ) {
            Column(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(20.dp),
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                ProfilePicPlaceholder(
                    placeholderSize = 120.dp,
                    borderWidth = 2.dp,
                    profilePictureUrl = "https://images.peopleimages.com/picture/202310/2874637-face-happiness-and-black-woman-with-glasses-silly-or-cheerful-girl-on-a-blue-studio-background.-portrait-african-person-or-model-with-funky-eyewear-trendy-or-goofy-with-casual-outfit-or-happiness-fit_400_400.jpg",
                    padding = 5.dp
                )
                Spacer(modifier = Modifier.height(10.dp))
                Text(
                    text = if (user == null || user.isAnonymous) "Anonymous" else user.name,
                    style = MaterialTheme.typography.bodyLarge
                )
                Spacer(modifier = Modifier.height(4.dp))
                Text(
                    text = if (user == null || user.isAnonymous) "anonymous@measuremate.io" else user.email ,
                    style = MaterialTheme.typography.bodySmall
                )
                Spacer(modifier = Modifier.height(20.dp))
                GoogleSignInButton(
                    onClick = onGoogleButtonClick,
                    loadingState = buttonLoadingState,
                    primaryText = buttonPrimaryText,
                )
            }
        }
    }
}