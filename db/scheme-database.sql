CREATE TABLE IF NOT EXISTS sensors (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    firmware_version TEXT NOT NULL,
    scale INTEGER NOT NULL,
    frequency INTEGER NOT NULL,
    threshold_stdv REAL NOT NULL,
    enable BOOLEAN NOT NULL CHECK (enable IN (0,1))
);