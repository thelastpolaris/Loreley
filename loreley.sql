-- phpMyAdmin SQL Dump
-- version 4.6.4deb1
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Feb 16, 2017 at 06:41 PM
-- Server version: 5.7.17-0ubuntu0.16.10.1
-- PHP Version: 7.0.13-0ubuntu0.16.10.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `loreley`
--

-- --------------------------------------------------------

--
-- Table structure for table `brands`
--

CREATE TABLE `brands` (
  `id` int(11) NOT NULL,
  `name` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `brands`
--

INSERT INTO `brands` (`id`, `name`) VALUES
(4, 'Mertex');

-- --------------------------------------------------------

--
-- Table structure for table `categories`
--

CREATE TABLE `categories` (
  `id` int(11) NOT NULL,
  `name` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `categories`
--

INSERT INTO `categories` (`id`, `name`) VALUES
(28, 'Блузки'),
(29, 'Рубашки');

-- --------------------------------------------------------

--
-- Table structure for table `category_sizes`
--

CREATE TABLE `category_sizes` (
  `category_sizes_id` int(11) NOT NULL,
  `category_id` int(11) DEFAULT NULL,
  `size_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `colors`
--

CREATE TABLE `colors` (
  `id` int(11) NOT NULL,
  `name` tinytext NOT NULL,
  `color` varchar(128) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `colors`
--

INSERT INTO `colors` (`id`, `name`, `color`) VALUES
(4, 'Красный', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `products`
--

CREATE TABLE `products` (
  `id` int(11) NOT NULL,
  `brand` int(11) NOT NULL,
  `category_id` int(11) NOT NULL,
  `name` varchar(256) NOT NULL,
  `color` int(11) NOT NULL,
  `price` decimal(10,0) NOT NULL,
  `note` mediumtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `products`
--

INSERT INTO `products` (`id`, `brand`, `category_id`, `name`, `color`, `price`, `note`) VALUES
(48, 4, 28, 'Джинса', 4, '32100', '312312'),
(49, 4, 28, '3333', 4, '23226', '');

--
-- Triggers `products`
--
DELIMITER $$
CREATE TRIGGER `delete_products_and_subproducts` BEFORE DELETE ON `products` FOR EACH ROW DELETE FROM subproducts WHERE subproducts.product_id = old.id
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `sells`
--

CREATE TABLE `sells` (
  `id` int(11) NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `price` double NOT NULL,
  `client` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `sells`
--

INSERT INTO `sells` (`id`, `date`, `price`, `client`) VALUES
(17, '2017-02-16 18:41:24', 55326, -1);

-- --------------------------------------------------------

--
-- Table structure for table `sells_subproducts`
--

CREATE TABLE `sells_subproducts` (
  `id` int(11) NOT NULL,
  `sell` int(11) NOT NULL,
  `subproduct` int(11) NOT NULL,
  `amount` int(11) NOT NULL DEFAULT '1',
  `discount` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `sells_subproducts`
--

INSERT INTO `sells_subproducts` (`id`, `sell`, `subproduct`, `amount`, `discount`) VALUES
(7, 17, 80, 1, 0),
(8, 17, 82, 1, 0);

-- --------------------------------------------------------

--
-- Table structure for table `sizes`
--

CREATE TABLE `sizes` (
  `id` int(11) NOT NULL,
  `name` tinytext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sizes`
--

INSERT INTO `sizes` (`id`, `name`) VALUES
(5, '38'),
(6, '32'),
(7, '36');

-- --------------------------------------------------------

--
-- Table structure for table `subproducts`
--

CREATE TABLE `subproducts` (
  `id` int(11) NOT NULL,
  `product_id` int(11) NOT NULL,
  `size` int(11) NOT NULL,
  `barcode` varchar(13) DEFAULT NULL,
  `note` mediumtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subproducts`
--

INSERT INTO `subproducts` (`id`, `product_id`, `size`, `barcode`, `note`) VALUES
(80, 49, 5, '2030000000791', NULL),
(82, 48, 6, '2030000000807', NULL);

--
-- Triggers `subproducts`
--
DELIMITER $$
CREATE TRIGGER `delete_subproduct` BEFORE DELETE ON `subproducts` FOR EACH ROW BEGIN
DELETE FROM subprod_arrival WHERE subprod_arrival.subprod_id = old.id;
DELETE FROM subprod_reduce WHERE subprod_reduce.subprod_id = old.id;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `subprod_arrival`
--

CREATE TABLE `subprod_arrival` (
  `id` int(11) NOT NULL,
  `subprod_id` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `arrival_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subprod_arrival`
--

INSERT INTO `subprod_arrival` (`id`, `subprod_id`, `amount`, `arrival_date`) VALUES
(26, 80, 1, '2016-12-03'),
(28, 82, 1, '2017-02-05'),
(29, 82, 1, '2017-02-05'),
(30, 80, 2, '2017-02-16');

-- --------------------------------------------------------

--
-- Table structure for table `subprod_reasons`
--

CREATE TABLE `subprod_reasons` (
  `id` int(11) NOT NULL,
  `name` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subprod_reasons`
--

INSERT INTO `subprod_reasons` (`id`, `name`) VALUES
(1, 'Ошибка'),
(3, 'Продажа');

-- --------------------------------------------------------

--
-- Table structure for table `subprod_reduce`
--

CREATE TABLE `subprod_reduce` (
  `id` int(11) NOT NULL,
  `subprod_id` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `reason` int(11) NOT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subprod_reduce`
--

INSERT INTO `subprod_reduce` (`id`, `subprod_id`, `amount`, `reason`, `date`) VALUES
(9, 80, 1, 3, '2017-02-16 18:41:24'),
(10, 82, 1, 3, '2017-02-16 18:41:24');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `brands`
--
ALTER TABLE `brands`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `category_sizes`
--
ALTER TABLE `category_sizes`
  ADD PRIMARY KEY (`category_sizes_id`);

--
-- Indexes for table `colors`
--
ALTER TABLE `colors`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `products`
--
ALTER TABLE `products`
  ADD PRIMARY KEY (`id`),
  ADD KEY `index_name` (`name`(255)) USING BTREE;

--
-- Indexes for table `sells`
--
ALTER TABLE `sells`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `sells_subproducts`
--
ALTER TABLE `sells_subproducts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `sizes`
--
ALTER TABLE `sizes`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subproducts`
--
ALTER TABLE `subproducts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subprod_arrival`
--
ALTER TABLE `subprod_arrival`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subprod_reasons`
--
ALTER TABLE `subprod_reasons`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subprod_reduce`
--
ALTER TABLE `subprod_reduce`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `brands`
--
ALTER TABLE `brands`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `categories`
--
ALTER TABLE `categories`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=30;
--
-- AUTO_INCREMENT for table `category_sizes`
--
ALTER TABLE `category_sizes`
  MODIFY `category_sizes_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `colors`
--
ALTER TABLE `colors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `products`
--
ALTER TABLE `products`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=50;
--
-- AUTO_INCREMENT for table `sells`
--
ALTER TABLE `sells`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=18;
--
-- AUTO_INCREMENT for table `sells_subproducts`
--
ALTER TABLE `sells_subproducts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT for table `sizes`
--
ALTER TABLE `sizes`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT for table `subproducts`
--
ALTER TABLE `subproducts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=83;
--
-- AUTO_INCREMENT for table `subprod_arrival`
--
ALTER TABLE `subprod_arrival`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=31;
--
-- AUTO_INCREMENT for table `subprod_reasons`
--
ALTER TABLE `subprod_reasons`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
--
-- AUTO_INCREMENT for table `subprod_reduce`
--
ALTER TABLE `subprod_reduce`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
