// Global Variables
let books = [];
let filteredBooks = [];
let currentBookId = null;
let currentAction = null;

// Load books on page load
document.addEventListener('DOMContentLoaded', function() {
    loadBooks();
});

// Load books from JSON file
function loadBooks() {
    fetch('data/books.json')
        .then(response => {
            if (!response.ok) {
                throw new Error('Failed to load books');
            }
            return response.json();
        })
        .then(data => {
            books = data;
            filteredBooks = [...books];
            displayBooks();
            hideLoadingMessage();
        })
        .catch(error => {
            console.error('Error loading books:', error);
            showLoadingMessage('Error loading books. Make sure data/books.json exists.');
        });
}

// Display books in table
function displayBooks() {
    const tableBody = document.getElementById('booksTableBody');
    const booksContainer = document.getElementById('booksTableContainer');
    const noBooksMessage = document.getElementById('noBooksMessage');
    const totalBooks = document.getElementById('totalBooks');

    if (filteredBooks.length === 0) {
        booksContainer.style.display = 'none';
        noBooksMessage.style.display = 'block';
        return;
    }

    booksContainer.style.display = 'block';
    noBooksMessage.style.display = 'none';

    tableBody.innerHTML = '';

    filteredBooks.forEach(book => {
        const row = document.createElement('tr');

        const statusBadge = book.available === 1
            ? '<span class="badge badge-available">✓ Available</span>'
            : '<span class="badge badge-borrowed">✗ Borrowed</span>';

        const borrowedInfo = book.available === 1
            ? ''
            : `<small class="text-muted d-block">(${book.borrowed_by})</small>`;

        const actionButtons = book.available === 1
            ? `<button class="btn btn-primary btn-sm me-1" onclick="openBorrowModal(${book.id}, '${book.title}')">Borrow</button>`
            : `<button class="btn btn-success btn-sm me-1" onclick="openReturnModal(${book.id}, '${book.title}', '${book.borrowed_by}')">Return</button>`;

        row.innerHTML = `
            <td><strong>${book.id}</strong></td>
            <td>
                <strong>${book.title}</strong>
                ${borrowedInfo}
            </td>
            <td>${book.author}</td>
            <td>${book.year}</td>
            <td>${statusBadge}</td>
            <td>
                ${actionButtons}
                <button class="btn btn-danger btn-sm" onclick="openDeleteModal(${book.id}, '${book.title}')">Delete</button>
            </td>
        `;

        tableBody.appendChild(row);
    });

    totalBooks.textContent = filteredBooks.length;
}

// Filter books by search and status
function filterBooks() {
    const searchTerm = document.getElementById('searchInput').value.toLowerCase();
    const statusFilter = document.getElementById('statusFilter').value;

    filteredBooks = books.filter(book => {
        const matchesSearch = book.title.toLowerCase().includes(searchTerm) ||
            book.author.toLowerCase().includes(searchTerm);
        const matchesStatus = statusFilter === '' || book.available.toString() === statusFilter;

        return matchesSearch && matchesStatus;
    });

    displayBooks();
}

// Reset filter
function resetFilter() {
    document.getElementById('searchInput').value = '';
    document.getElementById('statusFilter').value = '';
    filteredBooks = [...books];
    displayBooks();
}

// Add new book
function addBook() {
    const title = document.getElementById('titleInput').value.trim();
    const author = document.getElementById('authorInput').value.trim();
    const year = parseInt(document.getElementById('yearInput').value);

    // Validation
    if (!title || !author || !year) {
        alert('Please fill all fields!');
        return;
    }

    if (year < 1000 || year > new Date().getFullYear()) {
        alert('Please enter a valid year!');
        return;
    }

    // Create new book
    const newBook = {
        id: books.length > 0 ? Math.max(...books.map(b => b.id)) + 1 : 1,
        title: title,
        author: author,
        year: year,
        available: 1,
        borrowed_by: ""
    };

    // Add to array
    books.push(newBook);
    filteredBooks = [...books];

    // Clear form
    document.getElementById('titleInput').value = '';
    document.getElementById('authorInput').value = '';
    document.getElementById('yearInput').value = '';

    // Refresh display
    displayBooks();
    showAlert('Book added successfully!', 'success');

    // Save to backend (optional - commented out for now)
    // saveToBackend();
}

// Open borrow modal
function openBorrowModal(bookId, bookTitle) {
    currentBookId = bookId;
    currentAction = 'borrow';

    document.getElementById('borrowBookTitle').textContent = bookTitle;
    document.getElementById('memberName').value = '';

    const modal = new bootstrap.Modal(document.getElementById('borrowModal'));
    modal.show();
}

// Confirm borrow
function confirmBorrow() {
    const memberName = document.getElementById('memberName').value.trim();

    if (!memberName) {
        alert('Please enter your name!');
        return;
    }

    const book = books.find(b => b.id === currentBookId);
    if (book) {
        book.available = 0;
        book.borrowed_by = memberName;
        filteredBooks = [...books];
        displayBooks();

        const modal = bootstrap.Modal.getInstance(document.getElementById('borrowModal'));
        modal.hide();

        showAlert(`"${book.title}" borrowed by ${memberName}!`, 'success');
    }
}

// Open return modal
function openReturnModal(bookId, bookTitle, borrowedBy) {
    currentBookId = bookId;
    currentAction = 'return';

    document.getElementById('returnBookTitle').textContent = bookTitle;
    document.getElementById('borrowedByMember').textContent = borrowedBy;

    const modal = new bootstrap.Modal(document.getElementById('returnModal'));
    modal.show();
}

// Confirm return
function confirmReturn() {
    const book = books.find(b => b.id === currentBookId);
    if (book) {
        const previousMember = book.borrowed_by;
        book.available = 1;
        book.borrowed_by = "";
        filteredBooks = [...books];
        displayBooks();

        const modal = bootstrap.Modal.getInstance(document.getElementById('returnModal'));
        modal.hide();

        showAlert(`"${book.title}" returned (was borrowed by ${previousMember})!`, 'success');
    }
}

// Open delete modal
function openDeleteModal(bookId, bookTitle) {
    currentBookId = bookId;
    currentAction = 'delete';

    document.getElementById('deleteBookTitle').textContent = bookTitle;

    const modal = new bootstrap.Modal(document.getElementById('deleteModal'));
    modal.show();
}

// Confirm delete
function confirmDelete() {
    const bookIndex = books.findIndex(b => b.id === currentBookId);
    if (bookIndex > -1) {
        const deletedBook = books[bookIndex];
        books.splice(bookIndex, 1);
        filteredBooks = [...books];
        displayBooks();

        const modal = bootstrap.Modal.getInstance(document.getElementById('deleteModal'));
        modal.hide();

        showAlert(`"${deletedBook.title}" deleted!`, 'danger');
    }
}

// Show loading message
function showLoadingMessage(message = 'Loading books...') {
    const loadingMsg = document.getElementById('loadingMessage');
    loadingMsg.textContent = message;
    loadingMsg.style.display = 'block';
}

// Hide loading message
function hideLoadingMessage() {
    const loadingMsg = document.getElementById('loadingMessage');
    loadingMsg.style.display = 'none';
}

// Show alert message
function showAlert(message, type = 'info') {
    const alertDiv = document.createElement('div');
    alertDiv.className = `alert alert-${type} alert-dismissible fade show`;
    alertDiv.setAttribute('role', 'alert');
    alertDiv.innerHTML = `
        ${message}
        <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
    `;

    // Insert at the top of the books card
    const booksCard = document.querySelector('.card');
    booksCard.insertBefore(alertDiv, booksCard.firstChild);

    // Auto-dismiss after 4 seconds
    setTimeout(() => {
        alertDiv.remove();
    }, 4000);
}

// Search functionality (Real-time search)
document.getElementById('searchInput').addEventListener('keyup', function() {
    filterBooks();
});

// Status filter change
document.getElementById('statusFilter').addEventListener('change', function() {
    filterBooks();
});

// Export books to JSON (for download)
function exportBooks() {
    const dataStr = JSON.stringify(books, null, 2);
    const dataBlob = new Blob([dataStr], { type: 'application/json' });
    const url = URL.createObjectURL(dataBlob);
    const link = document.createElement('a');
    link.href = url;
    link.download = 'books.json';
    link.click();
}

// Import books from JSON (for upload)
function importBooks(file) {
    const reader = new FileReader();
    reader.onload = function(e) {
        try {
            const importedBooks = JSON.parse(e.target.result);
            if (Array.isArray(importedBooks)) {
                books = importedBooks;
                filteredBooks = [...books];
                displayBooks();
                showAlert('Books imported successfully!', 'success');
            } else {
                showAlert('Invalid JSON format!', 'danger');
            }
        } catch (error) {
            showAlert('Error parsing JSON file!', 'danger');
        }
    };
    reader.readAsText(file);
}