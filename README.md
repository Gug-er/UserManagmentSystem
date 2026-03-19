User Management System (UMS) is a study project written in C++ for managing users using an SQLite database

The project implements basic authentication and user administration features.


### Features
- User registration and authentication
- User data stored in SQLite database
- Role support (admin / regular user)
- Password hashing
- Logging system
- Console-based interface

### Requirements
- C++17 or higher
- CMake
- SQLite3
- Compiler (GCC / MinGW / MSVC)

### Build & Run

#### 1. Clone the repository

```
git clone https://github.com/Gug-er/UserManagmentSystem.git
cd UserManagmentSystem
```

#### 2. Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

#### 3. Run

```
./ums
```

(On Windows)

```
ums.exe
```

---

###  Database

The project uses an SQLite database `users.db`.

Example table structure:

```
	users
	(
	"id INTEGER PRIMARY KEY,"
	"isAdmin INTEGER,"
	"login TEXT UNIQUE,"
	"password_hash TEXT"
	)
```

### Security Notes

- Passwords are stored as hashes
- Prepared statements are used (protection against SQL injection)

### License

This project is for educational purposes. You can modify and use it freely.
