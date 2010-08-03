processInput();
updateLights();

function torusKnotValue(q, p, theta)
{
	var partial = ((2 + Math.cos((q * theta) / p)));

	var x = partial * Math.cos(theta);
	var y = partial * Math.sin(theta);
	var z = Math.sin((q * theta) / p);

	return new QVector3D(x * 1000, y * 1000, z * 1000);
}

function updateLights()
{
	objectStore.getObject('RedLight').position = torusKnotValue(2.0, 5.0, globals.currentFrameTime / 500);
	objectStore.getObject('GreenLight').position = torusKnotValue(3.0, 7.0, globals.currentFrameTime / 420);
	objectStore.getObject('BlueLight').position = torusKnotValue(3.5, 4.4, globals.currentFrameTime / 830);
}

function processInput()
{
	var pos = camera.position;
	var dir = -camera.zAxis;
	var right = camera.xAxis;
	if(keyboard.isPressed(Qt.Key_W))
	{
		camera.translate(dir);
	}
	if(keyboard.isPressed(Qt.Key_S))
	{
		camera.translate(-dir.x(), -dir.y(), -dir.z());
	}
	if(keyboard.isPressed(Qt.Key_A))
	{
		camera.translate(right);
	}
	if(keyboard.isPressed(Qt.Key_D))
	{		
		camera.translate(-right.x(), -right.y(), -right.z());
	}

	var milliSecondsElapsed = globals.currentFrameTime - globals.previousFrameTime;
	var secondsElapsed = milliSecondsElapsed / 1000;

	/*if(mouse.isPressed(Qt.RightButton))
	{
		mouseDeltaX = mouse.position.x() - mouse.previousPosition.x();
		camera.yaw(1);
		//camera.yaw(new OgreRadian(-mouseDeltaX * secondsElapsed));
	}*/
	if(mouse.isPressed(Qt.RightButton))
	{
		mouseDeltaX = mouse.position.x() - mouse.previousPosition.x();
		camera.yaw(-mouseDeltaX * secondsElapsed * 10);
	}
	if(mouse.isPressed(Qt.RightButton))
	{
		mouseDeltaY = mouse.position.y() - mouse.previousPosition.y();
		camera.pitch(-mouseDeltaY * secondsElapsed * 10);
	}
	
	wheelDelta = mouse.getWheelDelta();
	fov = camera.fieldOfView;
	tempFOV = -wheelDelta * 0.001;
	fov = fov + tempFOV;
	camera.fieldOfView = fov;
}