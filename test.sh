#!/bin/bash

# --- CONFIGURATION ---
MINISHELL="$(pwd)/minishell"
BASH="/bin/bash"
TMP_DIR=$(mktemp -d -t minishell_test_XXXXXX)

cleanup() {
    rm -rf "$TMP_DIR"
}
trap cleanup EXIT

color() {
    case $1 in
        green)  echo -e "\033[0;32m$2\033[0m" ;;
        red)    echo -e "\033[0;31m$2\033[0m" ;;
        yellow) echo -e "\033[1;33m$2\033[0m" ;;
        blue)   echo -e "\033[0;34m$2\033[0m" ;;
        *)      echo "$2" ;;
    esac
}

run_cmd() {
    local CMD="$1"
    local SHELL="$2"
    local PREFIX="$3"
    (
        cd "$TMP_DIR" || exit 1
        printf '%s\n' "$CMD" | "$SHELL" > "${PREFIX}_out.txt" 2> "${PREFIX}_err.txt"
        echo $? > "${PREFIX}_code.txt"
    )
}

clean_output() {
    local FILE="$1"
    local CMD="$2"
    # Echapper les / pour sed
    local SAFE_CMD=${CMD//\//\\/}
    sed -e "s/minishell$ //g" \
        -e "/^$SAFE_CMD$/d" \
        -e "/^$/d" \
        "$FILE" > "${FILE}.clean"
}

compare_cmd() {
    local CMD="$1"
    local TEST_NUM="$2"
    local TOTAL_TESTS="$3"

    printf "[%d/%d] Test : %s\n" "$TEST_NUM" "$TOTAL_TESTS" "$(color blue "$CMD")"

    run_cmd "$CMD" "$BASH" "bash"
    reset_tmp_dir       # <---- ON NETTOIE LA APRÈS bash
    run_cmd "$CMD" "$MINISHELL" "mini"
    reset_tmp_dir       # <---- ...ET ICI APRÈS minishell

    clean_output "$TMP_DIR/bash_out.txt" "$CMD"
    clean_output "$TMP_DIR/mini_out.txt" "$CMD"
    clean_output "$TMP_DIR/bash_err.txt" "$CMD"
    clean_output "$TMP_DIR/mini_err.txt" "$CMD"

    local OUT_DIFF
    local BASH_ERR=$(<"$TMP_DIR/bash_err.txt.clean")
    local MINI_ERR=$(<"$TMP_DIR/mini_err.txt.clean")
    local BASH_CODE=$(<"$TMP_DIR/bash_code.txt")
    local MINI_CODE=$(<"$TMP_DIR/mini_code.txt")

    if [[ "$CMD" == "env" ]]; then
        grep -vE '^(SHLVL|PWD|OLDPWD|_|HOME|USER)=' "$TMP_DIR/bash_out.txt.clean" > "$TMP_DIR/bash_env"
        grep -vE '^(SHLVL|PWD|OLDPWD|_|HOME|USER)=' "$TMP_DIR/mini_out.txt.clean" > "$TMP_DIR/mini_env"
        OUT_DIFF=$(diff -u "$TMP_DIR/bash_env" "$TMP_DIR/mini_env")
    else
        OUT_DIFF=$(diff -u "$TMP_DIR/bash_out.txt.clean" "$TMP_DIR/mini_out.txt.clean")
    fi

    local STATUS=0

    if [[ -n "$OUT_DIFF" ]]; then
        STATUS=1
        color red "  ❌ STDOUT DIFF"
        echo "$OUT_DIFF" | sed 's/^/    /'
    fi

	# Supprime toute ligne 'exit' à la fin du stderr de minishell, peu importe ce qu'il y a avant
    MINI_ERR="$(printf '%s' "$MINI_ERR" | sed '${/^exit$/d;}')"
    # Supprime tous \nexit potentiels restants à la fin s'il y a plusieurs \n
    while [[ "$MINI_ERR" =~ $'\nexit'$ ]]; do
        MINI_ERR="${MINI_ERR%$'\nexit'}"
    done
    # Si le résultat final est juste 'exit', on l'efface aussi
    if [[ "$MINI_ERR" == "exit" ]]; then
        MINI_ERR=""
    fi
    # Enlève les espaces ou sauts de ligne de fin qui résulteraient du sed
    MINI_ERR="${MINI_ERR%"${MINI_ERR##*[![:space:]]}"}"

    if [[ "$BASH_ERR" != "$MINI_ERR" ]]; then
        if [[ -n "$BASH_ERR" || -n "$MINI_ERR" ]]; then
            color yellow "  ⚠️  STDERR Mismatch"
            echo "    Bash stderr : '$BASH_ERR'"
            echo "    Mini stderr : '$MINI_ERR'"
        fi
    fi

    if [[ "$BASH_CODE" -ne "$MINI_CODE" ]]; then
        STATUS=1
        color yellow "  ⚠️  EXIT CODE mismatch"
        echo "    Bash: $BASH_CODE, Minishell: $MINI_CODE"
    fi

    if [[ $STATUS -eq 0 ]]; then
        color green "  ✅ OK"
        return 0
    else
        return 1
    fi
}

reset_tmp_dir() {
    # Tout supprimer SAUF les outputs de test
    find "$TMP_DIR" -mindepth 1 ! -name 'bash_out.txt' ! -name 'bash_err.txt' \
        ! -name 'bash_code.txt' ! -name 'bash_out.txt.clean' ! -name 'bash_err.txt.clean' \
        ! -name 'mini_out.txt' ! -name 'mini_err.txt' ! -name 'mini_code.txt' \
        ! -name 'mini_out.txt.clean' ! -name 'mini_err.txt.clean' \
        -exec rm -rf {} +

    # Bonus : supprimer aussi les fichiers cachés
    find "$TMP_DIR" -mindepth 1 -name '.*' -exec rm -rf {} +
}

TEST_CMDS=(
    # Commandes simples
    "echo Hello World"
    "echo -n hello"
    "echo        hello   world"
    "ls"
    "pwd"
    "invalid_command"
    "echo"
    ""

    # Variables et expansions simples
    "export X=test"
    "export Y=a"
    "export Z=b"
    "echo $Y $Z"
    "export V=initial"
    "export V=changed"
    "echo $V"
    "export A=1"
    "unset A"
    "echo $A"
    "echo $UNDEF"

    # Quotes simples, pas de backslash ni de double quote
    "echo A test"
    "echo text   V text"

    # Expansion sans quotes
    "echo $V test"
    "echo test $V test"

    # Cas d'erreur de syntaxe basiques
    "echo 'unclosed single quote"

    # Erreurs sur variables invalides (noms incorrects)
    "export 1VAR=bad"
    "export VAR-NAME=bad"
    "export =novar"
    "unset 123"

    # Redirections simples
    "echo hello > f1"
    "cat f1"
    "echo abc >> f1"
    "cat f1"
    "echo restart > f1"
    "echo ok > f2"
    "cat < f2"

    # Changement de dossier et erreurs
    "cd /"
    "pwd"
    "cd /tmp"
    "pwd"
    "cd no_such_dir"

    "unset V"
    "unset"

    # Cas limites
    "echo ''"
    "echo $V$Z"
    "echo $V$UNDEF"
    "echo "
    "echo      "
    ""

    # Réaffectation après unset
    "export TMP=foo"
    "unset TMP"
    "echo $TMP"
    "export TMP=bar"
    "echo $TMP"

    # Redirection simple supplémentaire
    "echo ok > file1"
    "cat file1"
    "echo plop > file2"
    "cat file1 file2"

    "cd unknown_dir"
    "ls unknown_file"

    # Expansion nulle
    "export EMPTY="
    "echo $EMPTY"

    "echo content > $EMPTY"
    "ls"

    # Remplacement pour suppression des tests avec backslash :
    # On rajoute quelques tests originaux, en remplacement:
    "echo test1"
    "echo test2"
    "echo 123 test"
    "echo fin de tests"

	    # Tests hardcore et cas limites supplémentaires

    # Variables unset/inconnues utilisées dans des chemins et redirections
    "touch foo"
    "cat foo > $UNDEF"
    "cat $UNDEF"

    # Redirection vers un fichier déjà ouvert (overwrite)
    "echo hello > xx"
    "echo world > xx"
    "cat xx"

    # Redirection vers /dev/null
    "ls > /dev/null"
    "ls /doesnotexist > /dev/null"

    # Redirections croisées, overwrite + append
    "echo A > a"
    "echo B >> a"
    "echo C > b"
    "cat a b"

    # Cat sur fichier inexistant (erreur)
    "cat fichier_inexistant"

    # Export et unset en cascade
    "export A=1"
    "export A=2"
    "export A=3"
    "echo $A"
    "unset A"
    "echo $A"

    # Plusieurs espaces/vides étranges au début et à la fin
    "      echo test_debut"
    "echo test_fin     "
    "   echo   test_milieu   "

    # Redirection avec un chemin inexistant
    "echo oops > /path/that/does/not/exist"

    # Touch, echo, ls avec filename très chelou
    "touch '.foo '"
    "ls '.foo '"

    # Sortie erreur utilisée en normal
    "ls /doesnotexist 2> errfile"
    "cat errfile"

    # chdir, pwd et echo concaténés dans un même dossier
    "mkdir xdir"
    "cd xdir"
    "pwd"
    "echo YES"
    "cd .."

    # Expansion de variables non définies dans différents contextes
    "echo z$UNDEFz"
    "export ZEE="
    "echo --$ZEE--"

    # Tentative de redirection vers un dossier
    "mkdir odir"
    "echo hello > odir"

    # Double unset
    "export Q=val"
    "unset Q"
    "unset Q"
    "echo $Q"

    # Fichier verrouillé (simulation simple)
    "touch lockme"
    "chmod 000 lockme"
    "cat lockme"
    "chmod 644 lockme"

    # Expansion multiple sur la même ligne
    "export Q1=abc"
    "export Q2=def"
    "echo $Q1$Q2 OK"
    "unset Q1"
    "export Q3=ghi"
    "echo $Q1$Q2$Q3 fin"

    # Redirection vers un même fichier plusieurs fois
    "echo 1 > repeat"
    "echo 2 >> repeat"
    "echo 3 >> repeat"
    "cat repeat"

    # Vide + espace + echo sans rien
    "    "
    ""

	"export a='1 b=2'"
	"export d=$a"
	"echo $d"
)

# --- EXECUTION ---
if [ ! -x "$MINISHELL" ]; then
    color red "Erreur : Le binaire '$MINISHELL' n'existe pas ou n'est pas exécutable."
    exit 1
fi

echo "--- DÉBUT DES TESTS ---"
echo "Binaire Minishell : $MINISHELL"
echo "Binaire Bash : $BASH"
echo "Répertoire temporaire : $TMP_DIR"
echo "------------------------"

PASSED=0
FAILED=0
TOTAL=${#TEST_CMDS[@]}

for i in "${!TEST_CMDS[@]}"; do
    CMD="${TEST_CMDS[$i]}"
    if compare_cmd "$CMD" $((i+1)) "$TOTAL"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
    echo
	reset_tmp_dir 
done

echo "------------------------"
color blue "Résumé des tests :"
color green "  ✅ Réussis : $PASSED / $TOTAL"
if [ "$FAILED" -gt 0 ]; then
    color red "  ❌ Échecs : $FAILED / $TOTAL"
fi
echo "------------------------"

exit "$FAILED"