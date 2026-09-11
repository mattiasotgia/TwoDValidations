echo "Working directory is ${PWD}"
echo "Setting up env..."

script="${BASH_SOURCE[0]}"
name="$(readlink -f "$script")"   # optional but recommended
base="$(dirname "$name")"

echo "Executing $name"
echo "Base dir in $base"

export FHICL_FILE_PATH=$base/fcl:$FHICL_FILE_PATH   
export FW_SEARCH_PATH=$base/xml:$FW_SEARCH_PATH

echo $FHICL_FILE_PATH
echo $FW_SEARCH_PATH