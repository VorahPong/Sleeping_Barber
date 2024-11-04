/*
    Name:           VorahPong, Mean
    Date:           Nov, 07, 2024
    Class:          CS3013
    Professor:      Chao Zhao
    Description:    Simulation of the Sleeping Barber problem using binary semaphore and thread. Sleeping Barber problem is a classic inter-process               communication and synchronization problem between multiple operating system processes. The problem is analogous to that of                    keeping a barber working when there are customers, resting when there are none, and doing so in an orderly manner.
 */
#include <iostream>     // for input, output
#include <semaphore>    // for binary semaphore
#include <thread>       // for utilizing thread
#include <vector>       // using vector
#include <chrono>       // for sleep()

/*
    Using binary semaphore
    std::binary_semaphore name(initilize number)
        0 for close
        1 for open
 
    .acquire() if the semaphore is 1 (available), it will decrement to 0 and continue. If semaphore not available, it will wait till it become available.
 
    .release() this will increment semaphore back to 1 and potentially unblocking other threats waiting on it.
 */

// global variables
    // stopping threads from messing with std::cout from other threads
    std::binary_semaphore printCout(1);


    // binary mutex
    std::binary_semaphore barber_is_ready(0);           // if 1 then the barber is ready to cut a hair
    std::binary_semaphore access_waiting_room_seats(1); // if 1 then waiting room can be incremented or decremented
    std::binary_semaphore customers_ready(0);           // if 1 then there is customer currently in the waiting room, ready to be served

    int numberOfFreeWRSeats = 0;                        // total number of seats in the waiting room
    int cut_time = 0;                                   // how long a hair cut take in seconds
    int customer_spawn_delay = 0;                       // delay between each customers spawning in seconds

//

// Barber actions
void* barber_thread(void* param) {
    int id = *(int *) param;
    // infinite loop
    while(true) {
        // check if there is a customer ready, else go to sleep
        if(customers_ready.try_acquire() == false) {
            printCout.acquire(); std::cout << "Barber number " << id << " is going to sleep.\n" << std::endl; printCout.release();
            
            customers_ready.acquire();
            
            printCout.acquire(); std::cout << "Barber number " << id << " is waking up.\n" << std::endl; printCout.release();
        }
        
        // change access WRSeats
        access_waiting_room_seats.acquire();
        numberOfFreeWRSeats += 1;                    // One waiting room chair becomes free
        
        barber_is_ready.release();                   // barbar is going busy
        access_waiting_room_seats.release();         // Don't need the lock on the chairs anymore
        
        // Cut hair
        printCout.acquire(); std::cout << "Barber number " << id << " is cutting hair.\n" << std::endl; printCout.release();
        std::this_thread::sleep_for(std::chrono::seconds(cut_time));
        printCout.acquire(); std::cout << "Barber number " << id << " has finished cutting hair.\n" << std::endl; printCout.release();
    }
}
//

// Customers action
void* customer_thread(void* param) {
    int id = *(int *) param;
    printCout.acquire(); std::cout << "Customer number " << id << " has entered.\n" << std::endl; printCout.release();
    // infinite loop to simulate multiple customers

    access_waiting_room_seats.acquire();        // try to get access to the waiting room
    
    // if there any free seats, sit down in a chair, and notify the barber, who's waiting
    if(numberOfFreeWRSeats > 0) {
        numberOfFreeWRSeats -= 1;
        customers_ready.release();              // customer is ready to get a hair cut whenever barbar ready
        
        access_waiting_room_seats.release();    // don't need to the lock the chairs anymore
        barber_is_ready.acquire();              // wait until the barber is ready
        // have hair cut
        printCout.acquire(); std::cout << "Customer number " << id << " is getting a haircut.\n" << std::endl; printCout.release();
        std::this_thread::sleep_for(std::chrono::seconds(cut_time));
        printCout.acquire(); std::cout << "Customer number " << id << " left with a good haircut.\n" << std::endl; printCout.release();
    }
    // otherweise, there are no free seats, tough luck!
    else {
        access_waiting_room_seats.release();    // release the lock on the seats
        // leave without a haircut
        printCout.acquire(); std::cout << "Customer number " << id << " leave without haircut.\n" << std::endl; printCout.release();
    }
    pthread_exit(0);
}
//

// Main
int main(int argc, const char * argv[]) {
    // create threads to simulate customers and barbers
    int barbers_size = 1;
    int customers_size = 4;
    
    // get inputs from user
    std::cout << "How many barbers are working in the shop? : ";
    std::cin >> barbers_size;
    std::cout << std::endl;
    
    std::cout << "How many customers are coming today? : ";
    std::cin >> customers_size;
    std::cout << std::endl;
    
    std::cout << "How long will a haircut take in second? : ";
    std::cin >> cut_time;
    std::cout << std::endl;
    
    std::cout << "How long a delay between each customers spawning in second? : ";
    std::cin >> customer_spawn_delay;
    std::cout << std::endl;
    
    std::cout << "Lastly, how many free seats in the waiting room for customers? : ";
    std::cin >> numberOfFreeWRSeats;
    std::cout << std::endl;
    //
    
    // storing threads in vectors
    std::vector<pthread_t> barbers(barbers_size);
    std::vector<pthread_t> customers(customers_size);
    //
    
    // Begin simulation
    std::cout << "*************************\n* Barber Shop is Open!! *\n*************************\n\n" << std::endl;
    
    for(int i = 0; i < barbers_size; i++) {
        int id = i+1;
        if(pthread_create(&barbers[i], nullptr, barber_thread, &id) != 0) {
            std::cout << "Error creating barber threads" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    for(int i = 0; i < customers_size; i++) {
        int id = i+1;
        if(pthread_create(&customers[i], nullptr, customer_thread, &id) != 0) {
            std::cout << "Error creating barber threads" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(customer_spawn_delay));
    }
    
    
    // wait till all customers are dealt with
        for(int i = 0; i < customers_size; i++) {
        pthread_join(customers[i], NULL);
    }
    std::cout << "No more customers for today!" << std::endl;
    //
    std::cout << "\n*************************\n* Barber Shop is Close! *\n*************************\n" << std::endl;
    
    // DONE, abandone the barber infinite loop
    exit(EXIT_SUCCESS);
}
//
