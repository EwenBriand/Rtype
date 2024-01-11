const { spawn } = require('child_process');

const childProcess = spawn('xterm', ['-e', './a.out']);

childProcess.stdout.on('data', (data) => {
    console.log(`Réponse de l'application C++ : ${data}`);
});

childProcess.stderr.on('data', (data) => {
    console.error(`Erreur de l'application C++ : ${data}`);
});

childProcess.on('error', (err) => {
    console.error(`Erreur lors du lancement de l'application C++ : ${err}`);
});

childProcess.on('close', (code) => {
    console.log(`L'application C++ s'est terminée avec le code ${code}`);
});