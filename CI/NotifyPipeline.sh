#!/bin/bash

set -e

POSITIONAL_ARGS=()
HELP_STR="
-h | --help                                     Show Help
-a | --auth-token <token>                       (Required) Personal Access Token
-d | --domain <domain name>                     (Required) Domain name for GoCD Server
-p | --port <port>                              GoCD Server Port
--pipeline-name <pipeline name>                 (Required) Pipename to trigger
-e | --environment <name> <value>               Environment Variable to add
--env-pair <name1>:<name2> <value1>:<value2>    Adding Environment Variables separated by colon
--http                                          Use http instead https
"

USE_HTTP=false

# List of environment variables
ENV_VARS_NAMES=()
ENV_VARS_VALUES=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            echo "$HELP_STR"
            exit 0
            ;;
        -a|--auth-token)
            TOKEN="$2"
            shift # past argument
            shift # past value
            ;;
        -d|--domain)
            DOMAIN="$2"
            shift # past argument
            shift # past value
            ;;
        -p|--port)
            PORT="$2"
            shift # past argument
            shift # past value
            ;;
        --pipeline-name)
            PIPELINE_NAME="$2"
            shift # past argument
            shift # past value
            ;;
        -e|--environment)
            ENV_VARS_NAMES+=("$2")
            
            CUR_ENV_VALUE=$(echo "$3" | sed ':a
                                            N
                                            $!ba
                                            s/\n/\\n/g
                                            s/\"/\\\"/g')
            
            ENV_VARS_VALUES+=("$CUR_ENV_VALUE")
            
            echo "Current Env Name: $2"
            echo "Current Env Value: $CUR_ENV_VALUE"
            
            shift # past argument
            shift # past value
            shift # past value
        #-t|--target)
        #    TARGET="$2"
        #    shift # past argument
        #    shift # past value
        #    ;;
        --env-pair)
            IFS=":" read CUR_NAME_1 CUR_NAME_2 SPLIT_EXCESS <<< "$2"
            # Check if there is exactly one colon in the argument string
            if [ "$CUR_NAME_1:$CUR_NAME_2" != "$2" ]; then
                echo "Error: Env-Pair should contain exactly one colon."
                echo "CUR_NAME_1: $CUR_NAME_1"
                echo "CUR_NAME_2: $CUR_NAME_2"
                echo "SPLIT_EXCESS: $SPLIT_EXCESS"
                exit 1
            fi
            ENV_VARS_NAMES+=("$CUR_NAME_1" "$CUR_NAME_2")
            
            IFS=":" read CUR_VALUE_1 CUR_VALUE_2 SPLIT_EXCESS <<< "$3"
            # Check if there is exactly one colon in the argument string
            if [ "$CUR_VALUE_1:$CUR_VALUE_2" != "$3" ]; then
                echo "Error: Env-Pair should contain exactly one colon."
                echo "CUR_VALUE_1: $CUR_VALUE_1"
                echo "CUR_VALUE_2: $CUR_VALUE_2"
                echo "SPLIT_EXCESS: $SPLIT_EXCESS"
                exit 1
            fi
            
            CUR_VALUE_1=$(echo "$CUR_VALUE_1" | sed ':a
                                                    N
                                                    $!ba
                                                    s/\n/\\n/g
                                                    s/\"/\\\"/g')
            
            CUR_VALUE_2=$(echo "$CUR_VALUE_2" | sed ':a
                                                    N
                                                    $!ba
                                                    s/\n/\\n/g
                                                    s/\"/\\\"/g')
            
            ENV_VARS_VALUES+=("$CUR_VALUE_1" "$CUR_VALUE_2")
            
            shift # past argument
            shift # past value
            shift # past value
            
            ;;
        --http)
            USE_HTTP=true
            shift # past argument
            ;;
        #--default)
        #  DEFAULT=YES
        #  shift # past argument
        #  ;;
        -*|--*)
            echo "Unknown option $1"
            exit 1
            ;;
        *)
            echo "Unknown field $1"
            exit 1
            #shift # past argument
            #POSITIONAL_ARGS+=("$1") # save positional arg
            ;;
    esac
done

# Check if all arguments are populated
if [ -z $TOKEN ] || [ -z $DOMAIN ] || [ -z $PIPELINE_NAME ]; then
    echo "Error: Not all arguments are provided."
    echo "TOKEN: $TOKEN"
    echo "DOMAIN: $DOMAIN"
    echo "PIPELINE_NAME: $PIPELINE_NAME"
    exit 1
fi

# Print the values of the environment variables
#echo "USER_STRING: $USER_STRING"
#echo "BRANCH_STRING: $BRANCH_STRING"

# Print all variables
#echo "TOKEN: $TOKEN"
#echo "DOMAIN: $DOMAIN"
#echo "PORT: $PORT"
#echo "PIPELINE_NAME: $PIPELINE_NAME"
#echo "TARGET: $TARGET"
#echo "USER_STRING: $USER_STRING"
#echo "BRANCH_STRING: $BRANCH_STRING"

HTTP_STR="$([ $USE_HTTP == true ] && echo "http" || echo "https")"
PORT_STR="$([ -z $PORT ] && echo "" || echo ":$PORT")"

# Assemble Environment Variables Json String
ENV_VARS_JSON=""
for (( i=0; i<${#ENV_VARS_NAMES[@]}; i++ )); do
    if [ $i -gt 0 ]; then
        ENV_VARS_JSON="$ENV_VARS_JSON, "
    fi
    
    ENV_VARS_JSON="$ENV_VARS_JSON
    {
        \"name\": \"${ENV_VARS_NAMES[$i]}\",
        \"secure\": false,
        \"value\": \"${ENV_VARS_VALUES[$i]}\"
    }"
done

ENV_VARS_JSON="\"environment_variables\": [${ENV_VARS_JSON}]"

curl --fail-with-body "$HTTP_STR://$DOMAIN$PORT_STR/go/api/pipelines/$PIPELINE_NAME/schedule" \
    -H "Authorization: Bearer $TOKEN" \
    -H 'Accept: application/vnd.go.cd.v1+json' \
    -H 'Content-Type: application/json' \
    -X POST \
    -d "{ \
            ${ENV_VARS_JSON}, \
            \"update_materials_before_scheduling\": true \
        }"

# Return success status
exit 0