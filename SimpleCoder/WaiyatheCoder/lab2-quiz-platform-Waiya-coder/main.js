document.addEventListener('DOMContentLoaded', () => {
    const introPage = document.getElementById('intro-page');
    const quizContainer = document.getElementById('quiz-container');
    const nextButton = document.getElementById('next-btn');
    const prevButton = document.getElementById('prev-btn');
    const startButton = document.getElementById('start-btn');
    const resultContainer = document.getElementById('result-overlay');
    let currentQuestionIndex = 0; // Renamed to match usage
    let quizData = [];
    let quizDataOutcomes = [];
    const userSelections = [];
    
    fetch('data/data.json')
        .then(response => response.json())
        .then(data => {
            console.log('Fetched data:', data);
            quizData = data.questions;
            quizDataOutcomes = data.outcomes;
           
            // displayQuestion();
        })
        .catch(err => console.error('Error fetching the questions:', err));
    
    
    function createNavigationButtons() {
            // Create Next Button
            const nextButton = document.createElement('button');
            nextButton.id = 'next-btn';
            nextButton.textContent = 'Next';
            nextButton.style.display = 'none'; // Initially hidden
            nextButton.addEventListener('click', () => {
                const selectedAnswer = document.querySelector('input[name="answer"]:checked');
                if (selectedAnswer) {
                    userSelections[currentQuestionIndex] = selectedAnswer.value;
                    currentQuestionIndex++;
                    displayQuestion();
                } else {
                    // make this alert spooky later
                    alert('Please select an answer before proceeding.');
                }
            });
    
            // Create Previous Button
            const prevButton = document.createElement('button');
            prevButton.id = 'prev-btn';
            prevButton.textContent = 'Previous';
            prevButton.style.display = 'none'; // Initially hidden
            prevButton.addEventListener('click', () => {
                if (currentQuestionIndex > 0) {
                    currentQuestionIndex--;
                    displayQuestion();
                }
            });
    
            // Append both buttons to the quiz container
            quizContainer.appendChild(prevButton);
            quizContainer.appendChild(nextButton);


            // Create Submit Button
            const submitButton = document.createElement('button');
            submitButton.id = 'submit-btn';
            submitButton.textContent = 'Submit';
            submitButton.style.display = 'none'; // Initially hidden
            submitButton.addEventListener('click', () => {
                
                calculateResult();
            });

            // create a restart button
            const restartButton = document.createElement('button');
            restartButton.id = 'restart-btn';
            restartButton.style.display = 'none';
            restartButton.textContent = 'Restart';

            restartButton.addEventListener('click', () => {
                currentQuestionIndex = 0;
                userSelections.length = 0;
                introPage.style.display = 'block';
            });

            quizContainer.appendChild(submitButton);
            quizContainer.appendChild(restartButton);

        }
    function displayQuestion() {
        // Clear the quiz container
        quizContainer.innerHTML = '';
        createNavigationButtons();
        if (currentQuestionIndex < quizData.length) {
            console.log('Displaying question', currentQuestionIndex);
            const questionData = quizData[currentQuestionIndex];
            console.log('Displaying questionData', questionData);


            // Create a div element to hold the question
            const questionElement = document.createElement('div');
            questionElement.classList.add('question');

            // Create the question text
            const questionText = document.createElement('h2');
            questionText.textContent = questionData.question_name;
            questionElement.appendChild(questionText);
            console.log('Displaying questionText', questionText);

            // Answers
            const answersList = document.createElement('ul');
            questionData.answers.forEach((answer, index) => {
                const answerItem = document.createElement('ul');
                const answerInput = document.createElement('input');
                
                // radio: Input for selecting a single option from a set.
                answerInput.type = 'radio';
                answerInput.name = 'answer'; // Fixed quote
                answerInput.value = answer.outcome;
                answerInput.id = `answer-${index}`;


                const answerLabel = document.createElement('label');
                answerLabel.htmlFor = answerInput.id;
                answerLabel.textContent = answer.text; // Corrected property name
                

                answerLabel.appendChild(answerInput);
                answerItem.appendChild(answerLabel);
                console.log('Displaying answerItem', answerItem);
                answersList.appendChild(answerItem); // Moved inside the loop
                console.log('Displaying answersList', answersList);


                answersList.addEventListener('change', (event) => {
                    if (event.target.name === 'answer') {
                        // Handle the selection
                        console.log(`Selected answer: ${event.target.value}`);
                    }
                });
                
            });

            questionElement.appendChild(answersList);
            if (quizContainer) {
                // Safe to manipulate quizContainer
                quizContainer.appendChild(questionElement);
            } else {
                console.error('Element with id "quiz-container" not found.');
            } 
            createNavigationButtons();

 
            document.getElementById('next-btn').style.display = currentQuestionIndex < quizData.length - 1 ? 'block' : 'none';
            document.getElementById('prev-btn').style.display = currentQuestionIndex > 0 ? 'block' : 'none';
            document.getElementById('submit-btn').style.display = currentQuestionIndex === quizData.length - 1 ? 'block' : 'none';
            document.getElementById('restart-btn').style.display =currentQuestionIndex === quizData.length - 1 ? 'block' : 'none';
        } else {
            // All questions have been answered
            // quizContainer.innerHTML = '<h2>Quiz Completed!</h2>';

            calculateResult();


        }
    }
    // function calculateResult() {
    //     // Ensure resultContent is properly declared and points to the correct DOM element
    //     const resultContent = document.querySelector('#result-overlay .overlay-content');
    //     if (!resultContent) {
    //         console.error("The result content element was not found.");
    //         return;
    //     }
    
    //     // Initialize house tally
    //     const houseTally = {
    //         Gryffindor: 0,
    //         Hufflepuff: 0,
    //         Ravenclaw: 0,
    //         Slytherin: 0
    //     };
    
    //     // Tally the selections
    //     userSelections.forEach(selection => {
    //         if (houseTally.hasOwnProperty(selection)) {
    //             houseTally[selection]++;
    //         } else {
    //             console.warn(`Invalid selection: ${selection}`);
    //         }
    //     });
    
    //     // Sort houses by tally count
    //     const sortedHouses = Object.keys(houseTally).sort((a, b) => houseTally[b] - houseTally[a]);
    //     const topHouse = sortedHouses[0];
    
    //     // Ensure that topHouse is a valid key in the outcomes object
    //     if (quizDataOutcomes && quizDataOutcomes.outcomes && quizDataOutcomes.outcomes[topHouse]) {
    //         const resultData = quizDataOutcomes.outcomes[topHouse];
    //         resultContent.innerHTML = `
    //             <h2>Congratulations! You've been sorted into ${topHouse}!</h2>
    //             <img src="${resultData.img}" alt="${topHouse}">
    //             <p>${resultData.text}</p>
    //         `;
    //     } else {
    //         console.error(`No data found for the house: ${topHouse}`);
    //     }
    
    //     // Display result overlay
    //     document.querySelector('#result-overlay').style.display = 'flex';
    //     document.getElementById('quiz-container').style.display = 'none';
    //     document.getElementById('restart-btn').style.display = 'block';
    // }
    
    function calculateResult() {
      
        const resultContent = document.querySelector('#result-overlay .overlay-content');
        if (!resultContent) {
            console.error("The result content element was not found.");
            return;
        }
    
        // Initialize house tally
        const houseTally = {
            Gryffindor: 0,
            Hufflepuff: 0,
            Ravenclaw: 0,
            Slytherin: 0
        };
    
        // Tally the selections
        userSelections.forEach(selection => {
            if (houseTally.hasOwnProperty(selection)) {
                houseTally[selection]++;
            } else {
                console.warn(`Invalid selection: ${selection}`);
            }
        });
        console.log('Final houseTally', houseTally);
    
        // Sort houses by tally count
        const sortedHouses = Object.keys(houseTally).sort((a, b) => houseTally[b] - houseTally[a]);
        console.log('Sorted houses', sortedHouses);
        const topHouse = sortedHouses[0];
        console.log('Top house', topHouse);


        console.log('QuizdataOutcoems:', quizDataOutcomes);
    
        if (quizDataOutcomes && quizDataOutcomes[topHouse]) {
    const resultData = quizDataOutcomes[topHouse];
    resultContent.innerHTML = `
    <div class="quiz-result">
    <img src="${resultData.img}" alt="${topHouse}">
    <div class="quiz-result-text">
        <h2>You've been Assigned the ${topHouse} House! </h2>
        <p>${resultData.text}</p>
    </div>
</div>
    `;
} else {
    console.error(`No data found for the house: ${topHouse}`);
    console.log("Available houses:", Object.keys(quizDataOutcomes));
}

    
        // Display result overlay
        document.querySelector('#result-overlay').style.display = 'flex';
        document.getElementById('quiz-container').style.display = 'none';
        document.getElementById('restart-btn').style.display = 'block';
    }
    
    
    
    

    startButton.addEventListener('click', () => {
        introPage.style.display = 'none';
        quizContainer.style.display = 'block';
        displayQuestion();
    });

    nextButton.addEventListener('click', () => {
        const selectedAnswer = document.querySelector('input[name="answer"]:checked');
        if (selectedAnswer) {
            // Process the selected answer if needed
            currentQuestionIndex++;
            displayQuestion();
        } else {
            alert('Please select an answer before proceeding.');
        }
    });

    prevButton.addEventListener('click', () => {
        if (currentQuestionIndex > 0) {
            currentQuestionIndex--;
            displayQuestion();
        }
    });



   
    
});
