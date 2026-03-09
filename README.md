 Library Management System - Frontend Setup Guide
🎯 Running the Frontend
There are 3 ways to run the frontend (HTML/CSS/JavaScript):

🚀 METHOD 1: VS Code Live Server (Easiest) ⭐
Step 1: Install Live Server Extension

Open VS Code
Press Extensions: Ctrl+Shift+X
Search "Live Server"
Install the one by ritwickdey

Step 2: Open Frontend Folder

File → Open Folder
Select library-system/frontend folder
Click OK

Step 3: Run

Right-click on index.html
Select "Open with Live Server"
✅ Browser opens automatically!

Result:
http://localhost:5500/

🐍 METHOD 2: Python HTTP Server
Step 1: Open Terminal
bashcd library-system/frontend
Step 2: Start Server
Python 3:
bashpython -m http.server 8000
Python 2:
bashpython -m SimpleHTTPServer 8000
```

### Step 3: Open in Browser
```
http://localhost:8000/

🟢 METHOD 3: Node.js HTTP Server
Step 1: Install http-server
bashnpm install -g http-server
Step 2: Start Server
bashcd library-system/frontend
http-server
```

### Step 3: Open in Browser
```
http://localhost:8080/
```

---

## 📁 File Structure (IMPORTANT!)

For the frontend to work correctly, files must be in correct locations:
```
frontend/
├── index.html              ← Main page
├── css/
│   └── style.css           ← Styling
├── js/
│   └── script.js           ← JavaScript logic
└── data/
    └── books.json          ← Book data
⚠️ books.json MUST be in frontend/data/ folder!

📊 Frontend Features
✅ Display Books - Table format
✅ Search - By title or author
✅ Filter - Available/Borrowed status
✅ Add Book - Form with validation
✅ Borrow Book - Modal dialog
✅ Return Book - Return functionality
✅ Delete Book - Remove from library
✅ Responsive - Mobile friendly design

🎨 Screen Layout
Left Panel (Sidebar)

🔍 Search Box
📊 Status Filter
➕ Add New Book Form

Right Panel (Main Content)

📋 Books Table
📱 Book Details
🔘 Action Buttons


⚡ Quick Start
3 simple steps:

Open frontend folder in VS Code
Right-click index.html → "Open with Live Server"
✅ Done!


🛠️ Troubleshooting
"Error loading books" message
Cause: books.json file not found
Solution:

Download books.json file
Place in frontend/data/ folder
Refresh page (F5)


Live Server not installed
Solution:

Go to VS Code Extensions (Ctrl+Shift+X)
Search and install "Live Server"
Restart VS Code


CORS Error (Backend connection)
Current: Frontend uses local data only
Future: Will connect to C backend via API

📌 Learning Points
What you'll learn from this project:

HTML5 structure
Bootstrap 5 framework
CSS styling & responsive design
JavaScript fetch API
Modal dialogs
Event handling
DOM manipulation
