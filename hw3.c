#include "hw3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Course
{
	int courseId;
	int enrolledStudents;
	char instructor[MAX_NAME_LEN];

} course;

typedef struct Classroom
{
	int capacity;
	course *weeklyProgram[WORKING_HOURS][WORKING_DAYS];

} classroom;

void checkInput(int agrc,
				char *argv[]);

void addProcess(course allCourses[MAX_COURSES]);

void printCourses(course allCourses[MAX_COURSES]);

course *find(course allCourses[MAX_COURSES],
			 int courseId);

int add(course allCourses[MAX_COURSES],
		int courseId,
		int enrolledStudents,
		char instructor[MAX_NAME_LEN]);

int schedule(classroom allClassrooms[MAX_ROOMS],
			 course *course,
			 int duration);

int main(int argc, char *argv[])
{

	checkInput(argc, argv);

	course allCourses[MAX_COURSES] = {{0, 0, {'\0'}}};
	classroom allClassrooms[MAX_ROOMS] = {{0, {{NULL}}}};
	char input;

	for (int i = 1; i < argc; i++)
	{
		allClassrooms[i - 1].capacity = atoi(argv[i]);
	}

	do
	{
		print_menu();

		scanf(" %c", &input);

		if (input == 'a')
		{
			addProcess(allCourses);
		}
		else if (input == 'c')
		{
			printCourses(allCourses);
		}
		else if (input == 's')
		{
			int courseId;
			int duration;

			printf("course duration: ");
			scanf("%d %d", &courseId, &duration);

			if (duration < 1)
			{
				printf("Invalid duration\n");
				continue;
			}

			course *courseToSchedule = find(allCourses, courseId);

			if (courseToSchedule == NULL)
			{
				printf("%d not found\n", courseId);
				continue;
			}

			int room;
			room = schedule(allClassrooms, courseToSchedule, duration);

			if (room == -1)
			{
				printf("%d not scheduled\n", courseId);
				continue;
			}

			printf("%d scheduled in %d\n", courseId, room);
		}

	} while (input != 'q');

	return 0;
}
void printCourses(course allCourses[MAX_COURSES])
{
	for (int i = 0; i < MAX_COURSES; i++)
	{
		if (allCourses[i].courseId != 0)
		{
			printf("[%d] %s %d\n",
				   allCourses[i].courseId,
				   allCourses[i].instructor,
				   allCourses[i].enrolledStudents);
		}
	}
}

int schedule(classroom allClassrooms[MAX_ROOMS], course *courseToSchedule, int duration)
{
	classroom currentClassroom;
	course *currentCourse;
	int day = -1;
	int time = -1;
	int isProfessorFree = 1;

	int roomIterator;

	for (roomIterator = 0; roomIterator < MAX_ROOMS; roomIterator++)
	{

		if (allClassrooms[roomIterator].capacity >= courseToSchedule->enrolledStudents)
		{
			currentClassroom = allClassrooms[roomIterator];

			for (int hourIterator = 0; hourIterator < WORKING_HOURS; hourIterator++)
			{
				for (int dayIterator = 0; dayIterator < WORKING_DAYS; dayIterator++)
				{
					if (currentClassroom.weeklyProgram[hourIterator][dayIterator] == NULL)
					{
						if (hourIterator + duration > WORKING_HOURS)
						{
							return -1;
						}
						for (int j = 0; j < duration - 1; j++)
						{

							if (currentClassroom.weeklyProgram[hourIterator + j][dayIterator] != NULL)
							{
								continue;
							}
						}

						day = dayIterator;
						time = hourIterator;

						for (int i = 0; i < MAX_ROOMS; i++)
						{

							currentCourse = allClassrooms[i].weeklyProgram[time][day];

							if (currentCourse != NULL)
							{
								if (strcmp(currentCourse->instructor, courseToSchedule->instructor) == 0)
								{
									isProfessorFree = 0;
									break;
								}
							}
						}

						if (isProfessorFree == 1)
						{
							for (int i = 0; i < duration; i++)
							{
								allClassrooms[roomIterator].weeklyProgram[time + i][day] = courseToSchedule;
							}
							return roomIterator;
						}
						else
						{
							isProfessorFree = 1;
						}
					}
				}
			}
		}
	}

	return -1;
}

void addProcess(course allCourses[MAX_COURSES])
{
	char professor[MAX_NAME_LEN];
	int courseId;
	int enrolledStudents;
	int addResult;

	printf("professor course students: ");
	scanf("%s %d %d", professor, &courseId, &enrolledStudents);

	addResult = add(allCourses,
					courseId,
					enrolledStudents,
					professor);

	if (addResult == -1)
	{
		printf("No space\n");
	}
	else if (addResult == 0)
	{
		printf("%d exists\n", courseId);
	}
	else if (addResult == 1)
	{
		printf("%d added\n", courseId);
	}
}

int add(course allCourses[MAX_COURSES], int courseId, int enrolledStudents, char instructor[MAX_NAME_LEN])
{
	course *course = find(allCourses, courseId);

	//Course does not exist and there is no free space
	if (course == NULL)
	{
		return -1;
	}

	//Course exists
	if (course->courseId == courseId)
	{
		return 0;
	}

	//Add course
	course->courseId = courseId;
	course->enrolledStudents = enrolledStudents;
	strcpy(course->instructor, instructor);

	return 1;
}

course *find(course allCourses[MAX_COURSES], int courseId)
{

	for (int i = 0; i < MAX_COURSES; i++)
	{
		if (allCourses[i].courseId == courseId)
		{
			return &allCourses[i];
		}
	}

	for (int i = 0; i < MAX_COURSES; i++)
	{
		if (allCourses[i].courseId == 0)
		{
			return &allCourses[i];
		}
	}

	return NULL;
}

void checkInput(int agrc, char *argv[])
{

	if (agrc > MAX_ROOMS + 1)
	{
		printf("Incorrect command-line arguments!\n");
		exit(1);
	}

	for (int i = 1; i < agrc; i++)
	{
		if (atoi(argv[i]) < 0)
		{
			printf("Incorrect command-line arguments!\n");
			exit(2);
		}
	}

	for (int i = 2; i < agrc; i++)
	{
		if (atoi(argv[i]) - atoi(argv[i - 1]) < 0)
		{
			printf("Incorrect command-line arguments!\n");
			exit(3);
		}
	}
}
