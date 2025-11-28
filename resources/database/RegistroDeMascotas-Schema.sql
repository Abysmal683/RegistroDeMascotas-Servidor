CREATE TABLE IF NOT EXISTS mascotas (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nombre TEXT NOT NULL,
    -- Permite mayúsculas
    raza TEXT NOT NULL,
    -- Será guardado en minúsculas
    sexo TEXT NOT NULL,
    -- m / h o macho / hembra (tú decides)
    peso REAL NOT NULL,
    -- kg con decimales
    edad INTEGER NOT NULL,
    -- años
    especie TEXT NOT NULL,
    -- perro, gato, ave, etc. en minúsculas
    foto BLOB NOT NULL,
    --imagen que indentifica a la mascota
    fecha_creacion TEXT DEFAULT CURRENT_TIMESTAMP
);
