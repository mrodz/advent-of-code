import java.io.*;

public class Solution {
	private static void part2() {
		try (FileReader inputFile = new FileReader("./input.txt")) {
			BufferedReader reader = new BufferedReader(inputFile);

			int firstCalories, secondCalories, thirdCalories;			
			firstCalories = secondCalories = thirdCalories = 0;

			String line;
			int currentCalories = 0;

			while ((line = reader.readLine()) != null) {
				if (line.equals("")) {
					if (currentCalories > firstCalories) {
						firstCalories = currentCalories;
					} else if (currentCalories > secondCalories) {
						thirdCalories = secondCalories;
						secondCalories = currentCalories;
					} else if (currentCalories > thirdCalories) {
						thirdCalories = currentCalories;
					}

					currentCalories = 0;
					continue;
				}

				currentCalories += Integer.parseInt(line);
			}

			System.out.printf("The top three elves carrying the most calories have a total of %d CC!", firstCalories + secondCalories + thirdCalories);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public static void main(String args[]) {
		part2();
	}
}