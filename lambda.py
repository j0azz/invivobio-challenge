import json
import boto3
from datetime import datetime

# Initialize AWS services
dynamodb = boto3.resource('dynamodb')
table_name = 'YourDynamoDBTableName'  # Replace with your DynamoDB table name
table = dynamodb.Table(table_name)

def lambda_handler(event, context):
    try:
        # Initialize the MQTT payload from the event
        mqtt_payload = json.loads(event['body'])
        
        # Extract data from the MQTT payload
        humidity = float(mqtt_payload.get('humidity'))
        temperature = float(mqtt_payload.get('temperature'))
    except (KeyError, ValueError) as e:
        return {
            'statusCode': 400,
            'body': json.dumps('Invalid payload format')
        }

    # Get the current timestamp
    timestamp = str(datetime.utcnow())

    # Store data in DynamoDB
    try:
        table.put_item(
            Item={
                'Timestamp': timestamp,
                'Humidity': humidity,
                'Temperature': temperature
            }
        )
        return {
            'statusCode': 200,
            'body': json.dumps('Data stored successfully')
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps('Error storing data: {}'.format(str(e)))
        }
