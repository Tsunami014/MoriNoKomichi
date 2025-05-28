# Mori no komichi (森の小道); Forest path
A todo app with many, many features.

# Sprint 1
In this sprint, the goal is to define the requirements and set out a layout and general idea for what the app will look like, encorporating some final UI to get a feel for what the outcome will look like. The app at this stage will have all the screens set out, with transitional and interactive elements completed. There will be some UI elements that are made to look pretty, to ensure that the looks are what is desired before continuing.
## Requirements outline
### Functional requirements
1. The user must be able to view, edit and delete tasks and subtasks that the system will store
2. The user must be able to view statistics on the time they spent studying.
3. The system must accept keyboard and mouse input to navigate the entire app.
4. The system must transparently monitor the user's activity to determine if they are on task when they have chosen to study.
### Non-functional requirements
1. The program must launch with a negligable delay, causing zero friction stopping the user from starting working on tasks.
2. The system needs to be fully functional all the time; including not requiring any internet connection and ensuring when an exception occurs, everything is smoothly processed for little user interruption.
3. The system must have clear labels and prompts and an onboarding page to be as accessable to users as possible.
## Specifications outline
### Functional specifications
1. The user must be able to create, edit and delete tasks and subtasks that the system keeps track of.
2. The user must be able to view detailed graphs of their study patterns.
3. The user must be able to study on a task and get user-defined rewards for the amount of time they spent on their tasks.
4. The user must be able to navigate all menues using their mouse and input values with their keyboard.
5. The user must be able to control an in-app character in world-building segments using their keyboard, and using their keyboard and mouse must be able to change the decorations in the world.
6. The system must be able to track what programs the user has open when studying and categorise them as productive or unproductive for an overall score.
7. The system will be implemented as a desktop app.
8. The program must have robust exception handling; and for errors that can occur, such as file not found and bad inputs (e.g. text instead of number), the program should prevent the problem from being as much of an inconvenience as possible; gracefully handling every issue, either fixing the problem or smoothly alerting the user and resuming or restarting the app's progress for a clean user experience.
### Non-functional specifications
1. The system must always launch with a delay or less than 0.5 seconds, and from startup there should be less than half a second of delay. This ensures user engagement and ease of use.
2. The user interface must follow a consistant and easily understandable interaction pattern. The menues must have a hierachial order, so the back button goes to the last page; helping the users not get lost.
3. All user interface elements must be clearly labelled
4. An onboarding process must explain how to use every feature of the app, ensuring all users will understand how to use the app. Additionally, the onboarding must be avaliable at any time if a user forgets.
## Use case diagram
## Storyboards
The storyboard is very large, so please view it online [here](https://excalidraw.com/#json=0pviVfWcTNLK0z1RRgB37,hJraR8767Ax1RVhMA-3DlQ)
## Data flow diagrams
### Level 0
![Data flow level 0 diagram](image.png)
### Level 1
![Data flow level 1 diagram](image-1.png)
## Gantt chart
```mermaid
gantt
    title Schedule
    dateFormat YYYY-MM-DD
    section Sprint 1
        Design idea                   :2025-05-04, 2025-05-07
        Requirements & specifications :2025-05-05, 2025-05-08
        Design                        :2025-05-04, 2025-05-11
        Build & test                  :2025-05-11, 2025-06-04
        Launch                        :milestone,  2025-06-04, 2m
        Review                        :2025-06-04, 2025-06-06
    section Sprint 2
        Design                        :2025-06-04, 2025-06-08
        Build & test                  :2025-06-08, 2025-06-30
        Launch                        :milestone,  2025-06-30, 2m
        Review                        :2025-06-30, 2025-07-02
    section Sprint 3
        Diagrams                      :2025-06-30, 2025-07-04
    section Sprint 4
        Design                        :2025-07-02, 2025-07-05
        Build & test                  :2025-07-05, 2025-07-16
        Launch                        :milestone,  2025-07-16, 2m
        Review                        :2025-07-16, 2025-07-18

```
(View [here](https://mermaid.live/view#pako:eNqllE1P4zAQhv-KNQdOSeV8pz6yFSd6WU6gXKx4mlokdtdxYHer_neclFILyq4CoxzGTp537NfO7KHWAoFBw5W1lSIurLQtkrt6i2Jo8TgnuMUbbTpuyb2LcL0OV6vjqx5rK7UidzsjlSXRcXaMFfayUUQK5ORjsJjGWUjdkwbkLS_O-E_8NUiDHSrbkyvS77CWG1nzsVrv4ZmHlx-qfxIXq0fe4q8H2QpX1mJv_4FH0QnPndQZv-WDqrefVu9k63S1woB4uJPq_P0_SXz-zwZybwMuzy-eSTzTlXei5UxXRuSMJ_RbriT0S65M2JQXIY0vupJ4rkjeGN71c0TTi6LpTKvH1Xmi2UyrC-_6F2GUf8PqEf-C1a_YKXd3BQJojBTArBkwgA5d2xiHsB-lK7Bb909XwFwquHmsoFIHx-y4etC6O2FGD80W2Ia3vRsNu7EDvR7T2yeoBJofelAWWDYpANvDb2AFXSyTIs2XaRmncVSWAfwBFufJIkuXUZIklBZlFGeHAP5ONemiLJwCCmm1WR9b4tQZDy8AMjCd))

Time split reasoning:
- Most of the time will be spent in sprints 1 and 2;
    - In sprint 1, we will create the entire app for all but functionality; the basics of the UI (with some fancy parts to ensure the design theme chosen is OK).
    - In sprint 2, we will make the app function by adding all the code that wasn't created in sprint 1, integrating it with the UI already created.
- Sprint 3 is just diagrams
- Sprint 4 is finishing touches - not much is to be done compared to the other sprints, the bulk of the app is already complete

# Sprint 2
In this sprint, we will make the app functional by implementing the chosen functions set out to be created in sprint 1, ensuring they connect with the UI created in sprint 1.
# Sprint 3
In this sprint, we will design OOP charts and diagrams.
# Sprint 4
In this sprint, we will add final touches and polish off the program, ensuring it's exactly as required. Additionally, 
